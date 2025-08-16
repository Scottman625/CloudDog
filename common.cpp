#include "common.h"
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QFile>
#include <QDebug>

Common* Common::m_instance = new Common;
Common::Common()
{

}

Common* Common::getInstance()
{
    return m_instance;
}

void Common::resetProjectRoot()
{
    // 重置靜態變量，強制重新計算路徑
    static QString* projectRoot = nullptr;
    static bool* initialized = nullptr;

    // 這是一個 hack 方法，但可以工作
    // 更好的方法是重構代碼
    qDebug() << "重置專案根目錄緩存";
}

QString Common::getProjectRoot()
{
    // 移除靜態變量，每次都重新計算
    qDebug() << "=== 開始獲取專案根目錄 ===";

    // 方法1: 使用應用程式路徑
    QString appDir = QCoreApplication::applicationDirPath();
    qDebug() << "應用程式路徑: " << appDir;

    if (!appDir.isEmpty()) {
        QDir projectDir(appDir);
        qDebug() << "開始從應用程式路徑回溯...";

        int steps = 0;
        while (!projectDir.isRoot() && steps < 15) {
            QString currentPath = projectDir.absolutePath();
            QString dirName = projectDir.dirName();
            qDebug() << QString("步驟 %1: %2 (目錄名: %3)").arg(steps).arg(currentPath).arg(dirName);

            // 檢查是否是 CloudDog 目錄
            if (dirName == "CloudDog") {
                qDebug() << "找到 CloudDog 目錄: " << currentPath;
                return QDir::toNativeSeparators(currentPath);
            }

            // 檢查是否包含專案標識文件
            if (projectDir.exists("CloudDog.pro") ||
                projectDir.exists("CMakeLists.txt") ||
                (projectDir.exists("conf") && projectDir.exists("src"))) {
                qDebug() << "根據專案文件確定根目錄: " << currentPath;
                return QDir::toNativeSeparators(currentPath);
            }

            if (!projectDir.cdUp()) {
                qDebug() << "無法繼續向上回溯";
                break;
            }
            steps++;
        }
    }

    // 方法2: 使用當前工作目錄
    QString workingDir = QDir::currentPath();
    qDebug() << "當前工作目錄: " << workingDir;

    if (!workingDir.isEmpty()) {
        QDir projectDir(workingDir);
        qDebug() << "開始從工作目錄回溯...";

        int steps = 0;
        while (!projectDir.isRoot() && steps < 15) {
            QString currentPath = projectDir.absolutePath();
            QString dirName = projectDir.dirName();
            qDebug() << QString("工作目錄步驟 %1: %2 (目錄名: %3)").arg(steps).arg(currentPath).arg(dirName);

            if (dirName == "CloudDog") {
                qDebug() << "在工作目錄找到 CloudDog: " << currentPath;
                return QDir::toNativeSeparators(currentPath);
            }

            if (projectDir.exists("CloudDog.pro") ||
                projectDir.exists("CMakeLists.txt") ||
                (projectDir.exists("conf") && projectDir.exists("src"))) {
                qDebug() << "在工作目錄根據專案文件確定根目錄: " << currentPath;
                return QDir::toNativeSeparators(currentPath);
            }

            if (!projectDir.cdUp()) {
                break;
            }
            steps++;
        }
    }

    // 方法3: 嘗試常見的專案路徑
    QStringList possibleRoots;
    possibleRoots << "D:/code/C++/CloudDog";
    possibleRoots << "D:/code/CloudDog";
    possibleRoots << "C:/code/C++/CloudDog";
    possibleRoots << "C:/code/CloudDog";
    possibleRoots << QDir::homePath() + "/CloudDog";
    possibleRoots << QDir::homePath() + "/code/CloudDog";

    qDebug() << "嘗試常見路徑...";
    for (const QString& path : possibleRoots) {
        qDebug() << "檢查路徑: " << path;
        if (QDir(path).exists()) {
            qDebug() << "找到有效路徑: " << path;
            return QDir::toNativeSeparators(path);
        }
    }

    // 方法4: 手動創建專案目錄（最後手段）
    QString fallbackPath = "D:/code/C++/CloudDog";
    qDebug() << "所有方法都失敗，嘗試創建目錄: " << fallbackPath;

    QDir fallbackDir;
    if (fallbackDir.mkpath(fallbackPath)) {
        qDebug() << "成功創建目錄: " << fallbackPath;

        // 創建必要的子目錄
        QDir projectDir(fallbackPath);
        projectDir.mkpath("conf");
        projectDir.mkpath("src");

        return QDir::toNativeSeparators(fallbackPath);
    }

    qDebug() << "=== 無法確定專案根目錄 ===";
    return "";
}


QString Common::getConfValue(QString title, QString key, QString path)
{
    QString projectRoot = getProjectRoot();
    if (projectRoot.isEmpty()) {
        qDebug() << "無法獲取專案根目錄";
        return "";
    }

    QString fullPath = projectRoot + "/" + path;
    qDebug() << "完整配置文件路徑: " << fullPath;

    QFile file(fullPath);
    if (!file.exists()) {
        qDebug() << "文件不存在: " << fullPath;
        return "";
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "打開配置文件失敗: " << fullPath;
        return "";
    }

    QByteArray json = file.readAll();
    file.close();

    if (json.isEmpty()) {
        qDebug() << "JSON 文件內容為空";
        return "";
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "JSON 解析錯誤: " << err.errorString();
        return "";
    }

    if (doc.isObject()) {
        QJsonObject rootObj = doc.object();
        QJsonValue titleValue = rootObj.value(title);
        if (titleValue.isObject()) {
            QJsonObject titleObj = titleValue.toObject();
            if (titleObj.contains(key)) {
                return titleObj.value(key).toString();
            }
        }
    }
    return "";
}


void Common::writeLoginInfo(QString user, QString pwd, bool isRemeber, QString path)
{
    QString ip = getConfValue("web_server", "ip", "conf.json");
    QString port = getConfValue("web_server", "port", "conf.json");
    qDebug() << "ip:" << ip << ",port:" << port;
    QMap<QString, QVariant> web_server;
    web_server.insert("ip", ip);
    web_server.insert("port", port);



    // login
    // 做兩次加密
    // 1. 進行des加密
    // 2. 進行base64加密

    // 第一步: 進行des加密
    // 用戶名
    unsigned char encUsr[1024] = {0};
    int encUsrLen;
    // toLocal8Bit, 轉換為本地字符集，如果是windows則為gbk編碼，如果linux則為utf-8
    int ret = DesEnc((unsigned char*)user.toLocal8Bit().data(),user.toLocal8Bit().length(),encUsr ,&encUsrLen);
    if(ret != 0){
        // 加密失敗
        qDebug() << "DesEnc 加密失敗";
        return;
    }
    // 密碼
    unsigned char encPwd[1024] = {0};
    int encPwdLen;

    ret = DesEnc((unsigned char*)pwd.toLocal8Bit().data(),pwd.toLocal8Bit().length(),encPwd ,&encPwdLen);
    if(ret != 0){
        // 加密失敗
        qDebug() << "DesEnc 加密失敗";
        return;
    }

    // 第二步: 進行base64加密
    QString base64User = QByteArray((char *)encUsr, encUsrLen).toBase64();
    QString base64Pwd = QByteArray((char *)encPwd, encPwdLen).toBase64();

    QMap<QString, QVariant> login;
    login.insert("pwd", base64Pwd);
    login.insert("user", base64User);
    if(isRemeber == true){
        login.insert("remember", CONF_REMEBER_YES);
    }else{
        login.insert("remember", CONF_REMEBER_NO);
    }

    QMap<QString, QVariant> json;
    json.insert("login", login);
    json.insert("web_server", web_server);

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(json);
    if(jsonDocument.isNull()==true){
        qDebug() << "QJsonDocument::fromVariant 錯誤";
        return;
    }

    // 保存到文件
    QFile file(path);
    qDebug() << path;
    if(false == file.open(QIODevice::WriteOnly)){
        qDebug() << "文件打開失敗";
    }else{
        file.write(jsonDocument.toJson());
        file.close();
    }
}

 QString Common::getStrMd5(QString str)
 {
     QByteArray arr;
     arr = QCryptographicHash::hash(str.toLocal8Bit(), QCryptographicHash::Md5);

     return arr.toHex();
 }

 // 伺服器信息，寫入配置文件
 void Common::writeWebInfo(QString ip, QString port, QString path)
 {
     // web_server信息
     QMap<QString, QVariant> web_server;
     web_server.insert("ip", ip);
     web_server.insert("port", port);

     // login信息
     QString user = getConfValue("login", "user");
     QString pwd = getConfValue("login", "pwd");
     QString remember = getConfValue("login", "remember");

     QMap<QString, QVariant> login;
     login.insert("user", user);
     login.insert("pwd", pwd);
     login.insert("remember", remember);

     QMap<QString, QVariant> json;
     json.insert("web_server", web_server);
     json.insert("login", login);

     QJsonDocument jsonDocument = QJsonDocument::fromVariant(json);
     if(jsonDocument.isNull() == true)
     {
         qDebug() << "QJsonDocument::fromVariant錯誤";
         return;
     }

     qDebug() << path;

     path = getProjectRoot() + "/" + path;
     qDebug() << "完整文件路徑: " << path;
     QFile file(path);
     if (false == file.open(QIODevice::WriteOnly))
     {
         qDebug() << "file open err";
         return;
     }
     file.write(jsonDocument.toJson());
     file.close();

 }
