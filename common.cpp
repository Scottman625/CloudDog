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
Common::Common(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager();
    getFileTypeList();
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

 QString Common::getFileMd5(QString filePath)
 {
     QFile localFile(filePath);

     if (!localFile.open(QFile::ReadOnly))
     {
         qDebug() << "file open error.";
         return 0;
     }

     QCryptographicHash ch(QCryptographicHash::Md5);

     quint64 totalBytes = 0;
     quint64 bytesWritten = 0;
     quint64 bytesToWrite = 0;
     quint64 loadSize = 1024 * 4;
     QByteArray buf;

     totalBytes = localFile.size();
     bytesToWrite = totalBytes;

     while (1)
     {
         if(bytesToWrite > 0)
         {
             buf = localFile.read(qMin(bytesToWrite, loadSize));
             ch.addData(buf);
             bytesWritten += buf.length();
             bytesToWrite -= buf.length();
             buf.resize(0);
         }
         else
         {
             break;
         }

         if(bytesWritten == totalBytes)
         {
             break;
         }
     }

     if (localFile.isOpen()) {
         localFile.close();
     }
     QByteArray md5 = ch.result();
     return md5.toHex();
 }

#include <QRandomGenerator>
#include <QDebug>

 QString Common::getBoundary() {
     // 隨機生成16個字符
     const char randoms[] = {'0','1','2','3','4','5','6','7','8','9',
                             'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                             'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

     QString temp;
     int len = sizeof(randoms) / sizeof(randoms[0]); // 使用 sizeof 而不是 strlen

     for (int i = 0; i < 16; i++) {
         int randIndex = QRandomGenerator::global()->bounded(len);
         temp += randoms[randIndex]; // 使用 += 而不是 []
     }

     qDebug() << "temp:" << temp;

     QString boundary = "------WebKitFormBoundary" + temp;
     return boundary;
 }


 QNetworkAccessManager* Common::getNetworkAccessManager()
 {
     return m_manager;
 }

 //读取conf/fileType文件夹，得到文件夹中的所有文件名，保存在m_fileTypeList
 void Common::getFileTypeList()
 {
     QDir dir(FILE_TYPE_DIR);
     if (!dir.exists()) {
         dir.mkpath(FILE_TYPE_DIR);
         qDebug() << FILE_TYPE_DIR << "创建成功";
     }

     /*
            QDir::Dirs      列出目录；
            QDir::AllDirs   列出所有目录，不对目录名进行过滤；
            QDir::Files     列出文件；
            QDir::Drives    列出逻辑驱动器名称，该枚举变量在Linux/Unix中将被忽略；
            QDir::NoSymLinks        不列出符号链接；
            QDir::NoDotAndDotDot    不列出文件系统中的特殊文件.及..；
            QDir::NoDot             不列出.文件，即指向当前目录的软链接
            QDir::NoDotDot          不列出..文件；
            QDir::AllEntries        其值为Dirs | Files | Drives，列出目录、文件、驱动器及软链接等所有文件；
            QDir::Readable      列出当前应用有读权限的文件或目录；
            QDir::Writable      列出当前应用有写权限的文件或目录；
            QDir::Executable    列出当前应用有执行权限的文件或目录；
            Readable、Writable及Executable均需要和Dirs或Files枚举值联合使用；
            QDir::Modified      列出已被修改的文件，该值在Linux/Unix系统中将被忽略；
            QDir::Hidden        列出隐藏文件；
            QDir::System        列出系统文件；
            QDir::CaseSensitive 设定过滤器为大小写敏感。
        */
     dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot | QDir::NoSymLinks);
     dir.setSorting(QDir::Size); //排序

     //遍历文件夹(conf/fileType)
     QFileInfoList fileInfoList = dir.entryInfoList();
     for (int i=0;i<fileInfoList.size(); i++) {
         QFileInfo fileInfo = fileInfoList.at(i);
         m_fileTypeList.append(fileInfo.fileName());
     }
 }

 //根据文件名，从m_fileTypeList查找，如果能找到，返回此类型，如果不能找到,返回other.png
 //fileTypeName  cpp.png/dll.png
 QString Common::getFileType(QString fileTypeName)
 {
     if (m_fileTypeList.contains(fileTypeName))
     {
         //如果能找到
         return fileTypeName;
     } else {
         return "other.png";
     }
 }

 void Common::writeRecord(QString user, QString fileName, QString code, QString path /*="conf/record"*/)
 {
     // conf/record/milo.txt
     QString userFilePath = QString("%1/%2.txt").arg(path).arg(user);
     qDebug() << "userFilePath:" << userFilePath;

     QDir dir(path);
     if (!dir.exists()) {
         //目录不存在,创建目录
         if (dir.mkpath(path)) {
             qDebug() << "目录创建成功";
         } else {
             qDebug() << "目录创建失败";
         }
     }

     QByteArray array;
     QFile file(userFilePath);
     if (file.exists()) {
         //如果存在, 先读取文件原来的内容

         if (!file.open(QIODevice::ReadOnly)) {
             //如果打开失败
             qDebug() << "file.open(QIODevice::ReadOnly) err";
             if (file.isOpen()) {
                 file.close();
             }
             return;
         }

         array = file.readAll();

         if (file.isOpen()) {
             file.close();
         }
     }

     if (!file.open(QIODevice::WriteOnly)) {
         //如果打开失败
         qDebug() << "file.open(QIODevice::WriteOnly) err";
         if (file.isOpen()) {
             file.close();
         }
         return;
     }

     //记录写入文件
     //xxx.jpg   2020/11/20 16:02:01 上传成功

     QDateTime time = QDateTime::currentDateTime(); //获取系统当前时间
     QString timeStr = time.toString("yyyy/MM/dd hh:mm:ss"); //时间格式化
     QString actionString = getActionStrring(code);

     //记录到文件的内容
     QString str = QString("%1\t%2\t%3\r\n").arg(fileName).arg(timeStr).arg(actionString);
     qDebug() << "str:" << str;

     //toLocal8Bit转为本地字符串
     file.write(str.toLocal8Bit());
     if (!array.isEmpty()) {
         //读取到文件内容不为空的时候
         file.write(array);
     }

     if (file.isOpen()) {
         file.close();
     }
 }

 QString Common::getActionStrring(QString code) {
     /*
    005：上传的文件已存在
    006: 秒传成功
    007: 秒传失败
    008: 上传成功
    009: 上传失败
    090: 下载成功
    091: 下载失败
    */
     QString str;
     if (code == "005") {
         str = "上传的文件已存在";
     } else if (code == "006") {
         str = "秒传成功";
     } else if (code == "007") {
         str = "秒传失败";
     } else if (code == "008") {
         str = "上传成功";
     } else if (code == "009") {
         str = "上传失败";
     } else if (code == "090") {
         str = "下载成功";
     } else if (code == "091") {
         str = "下载失败";
     }

     return str;
 }


 void Common::sleep(unsigned int msec)
 {
     QTime dieTime = QTime::currentTime().addMSecs(msec);
     while (QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
     }
 }
