#include "logindialog.h"
#include "ui_logindialog.h"
#include "logininfoinstance.h"
#include <QPainter>
#include <QDebug>
#include <QRegularExpression>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetWorkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include "common.h"
#include <QDir>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_mainwindow(nullptr)  // 初始化為 nullptr
{
    ui->setupUi(this);

    m_common = Common::getInstance();

    // 創建 MainWindow 實例
    m_mainwindow = new MainWindow();

    // 連接信號
    connect(m_mainwindow, &MainWindow::sigChangeUser, this, &LoginDialog::show);
    connect(m_mainwindow, &MainWindow::sigLoginAgain, this, &LoginDialog::show);

    //去掉標題欄
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //如果設置了樣式表，則不生效
    //this->setFont可以設置當前窗口所有的字體，loginDialog窗口中的所有控件都生效
    this->setFont(QFont("楷體",12,QFont::Bold,false));

    // ui->stackedWidget->currentIndex()
    // QObject::connect(信號對象, 信息函數, 槽對象, 槽函數)
    QObject::connect(ui->title_widget, &TitleWidget::showSetPage, this, &LoginDialog::showSetPage);
    QObject::connect(ui->title_widget, &TitleWidget::closeButton, this, &LoginDialog::closeButton);

    ui->stackedWidget->setCurrentIndex(0);

    qDebug() << "開始讀取配置文件";
    // 讀取cfg.json文件
    readConf();
    qDebug() << "初始化完成";
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::paintEvent(QPaintEvent *event){
    QPainter p(this);

    QPixmap bg(":/images/login_bk.jpg");
    p.drawPixmap(0,0,this->width(),this->height(),bg);
}

void LoginDialog::readConf()
{
    QString user = m_common->getConfValue("login", "user");
    QString pwd = m_common->getConfValue("login", "pwd");
    QString remeber = m_common->getConfValue("login", "remember");
    // qDebug() << "當前工作目錄:" << QDir::currentPath();
    int ret = 0;
    if(remeber == CONF_REMEBER_YES){
        //記住密碼

        //解密
        //1. base64解密
        //2. des解密

        // 第一步: base64解密
        //toLocal8Bit(),轉換為本地字符串，如果windows則為gbk編碼，如果是linux則為utf-8編碼
        QByteArray pwdTemp = QByteArray::fromBase64(pwd.toLocal8Bit());

        unsigned char encPwd[512] = {0};
        int encPwdLen = 0;
        // 第二步: des解密
        ret = DesDec((unsigned char*)pwdTemp.data(), pwdTemp.size(),encPwd, &encPwdLen);
        if(ret != 0){
            qDebug() << "解密失敗";
        }

        QString password = QString::fromLocal8Bit((const char*)encPwd, encPwdLen);
        qDebug() << "password" << password;
        ui->chk_remember->setChecked(true);
        ui->password->setText(password);
    }else{
        //沒有記住密碼

        ui->chk_remember->setChecked(false);
        ui->password->setText("");
    }

    QByteArray userTemp = QByteArray::fromBase64(user.toLocal8Bit());

    unsigned char encUsr[512] = {0};
    int encUsrLen = 0;
    // 第二步: des解密
    ret = DesDec((unsigned char*)userTemp.data(), userTemp.size(),encUsr, &encUsrLen);
    if(ret != 0){
        qDebug() << "解密失敗";
    }

    QString userName = QString::fromLocal8Bit((const char*)encUsr, encUsrLen);
    qDebug() << "userName" << userName;
    ui->username->setText(userName);
}

void LoginDialog::on_notReg_clicked()
{
    // 顯示註冊頁
    ui->stackedWidget->setCurrentWidget(ui->reg_page);
}

void LoginDialog::serverSet()
{
    QString ip = m_common->getConfValue("web_server", "ip");
    QString port = m_common->getConfValue("web_server", "port");
    qDebug() << "ip:" << ip << ",port" << port;
    ui->server_ip->setText(ip);
    ui->server_port->setText(port);
}


void LoginDialog::showSetPage()
{
    // 顯示設置頁
    ui->stackedWidget->setCurrentWidget(ui->set_page);
    serverSet();
}

void LoginDialog::closeButton()
{
    if(ui->stackedWidget->currentIndex()==0){ // 0是代表login_page
        this->close();
    }else{
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    }
}

void LoginDialog::saveLoginInfoData(QString username, QString token, QString ip, QString port) {
    //跳轉到其他頁面
    //保存數據, token, user, ip, 端口
    //除了登入外：每一個請求都需要校驗token，每一個請求都需要帶token
    LoginInfoInstance *loginInfo = LoginInfoInstance::getInstance();
    qDebug() << "token:" << token;

    loginInfo->setUser(username);
    loginInfo->setToken(token);
    loginInfo->setIp(ip);
    loginInfo->setPort(port);
}

void LoginDialog::on_login_clicked()
{
    qDebug() << "start login...";
    QString username = ui->username->text();
    QString password = ui->password->text();
    QRegularExpressionMatch match;

    // 驗證用戶名
    QRegularExpression regexp(USER_REG);
    match = regexp.match(username);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "警告", "用戶名格式不正確");
        ui->username->clear();
        ui->username->setFocus();
        return;
    }

    // 驗證密碼
    regexp.setPattern(PASSWD_REG);
    match = regexp.match(password);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "警告", "密碼格式不正確");
        ui->password->clear();
        ui->password->setFocus();
        return;
    }

    // 處理 HTTP 請求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    QString ip = m_common->getConfValue("web_server", "ip");
    QString port = m_common->getConfValue("web_server", "port");
    QString url = QString("http://%1:%2/login").arg(ip).arg(port);

    qDebug() << "Request URL:" << url;  // 調試輸出 URL

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject paramsObj;
    paramsObj.insert("user", username);
    paramsObj.insert("pwd", m_common->getStrMd5(password));
    QJsonDocument doc(paramsObj);
    QByteArray data = doc.toJson();

    qDebug() << "Request data:" << data;  // 調試輸出請求數據

    QNetworkReply *reply = manager->post(request, data);

    // 添加錯誤處理
    connect(reply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError error){
        qDebug() << "Network error:" << error;
        qDebug() << "Error string:" << reply->errorString();
        QMessageBox::critical(this, "網絡錯誤", "請求失敗: " + reply->errorString());
    });

    // 讀取伺服器返回的數據
    connect(reply, &QNetworkReply::finished, this, [=](){
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Request failed:" << reply->errorString();
            return;
        }

        QByteArray data = reply->readAll();
        qDebug() << "伺服器返回數據:" << QString(data);

        // 檢查返回的內容類型
        QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Content-Type:" << contentType;

        // 如果返回的是 HTML，說明路由配置有問題
        if (data.contains("<html>") || data.contains("404 Not Found")) {
            QMessageBox::critical(this, "服務器錯誤", "服務器配置錯誤，請檢查後端服務");
            reply->deleteLater();
            return;
        }

        QJsonParseError err;
        QJsonDocument rootDoc = QJsonDocument::fromJson(data, &err);

        if (err.error != QJsonParseError::NoError) {
            qDebug() << "Json格式錯誤:" << err.errorString();
            QMessageBox::critical(this, "數據錯誤", "服務器返回數據格式錯誤");
        } else {
            QJsonObject rootObj = rootDoc.object();
            QJsonValue codeValue = rootObj.value("code");

            if (codeValue.type() == QJsonValue::String) {
                QString code = codeValue.toString();

                if (code == "000") {
                    QMessageBox::information(this, "提示", "登入成功");
                    bool isCheck = ui->chk_remember->isChecked();
                    if (!isCheck) {
                        ui->password->setText("");
                    }
                    m_common->writeLoginInfo(username, password, isCheck);
                    // 跳轉到其他頁面
                    //保存用户資料
                    //獲取token
                    QJsonValue tokenValue = rootObj.value("token");
                    saveLoginInfoData(username, tokenValue.toString(), ip, port);

                    //跳到主界面,并初始化
                    m_mainwindow->init(username);
                    m_mainwindow->show();
                    this->hide();
                } else if (code == "001") {
                    QMessageBox::critical(this, "錯誤", "登入失敗");
                } else {
                    QMessageBox::critical(this, "錯誤", "未知錯誤代碼: " + code);
                }
            }
        }

        reply->deleteLater();
    });
}


void LoginDialog::on_btn_reg_clicked()
{
    // 用戶註冊
    QString username = ui->reg_username->text();
    QString nickname = ui->reg_nickname->text();
    QString pwd = ui->reg_pwd->text();
    QString pwd2 = ui->reg_pwd2->text();
    QString phone = ui->reg_phone->text();
    QString email = ui->reg_email->text();
    // 用戶名校驗
    QRegularExpression regUserName(USER_REG);
    QRegularExpressionMatch match = regUserName.match(username);  // 使用 match 方法進行匹配
    if (!match.hasMatch()) {  // 檢查是否匹配成功
        QMessageBox::warning(this, "警告","用戶名格式不正確");
        ui->username->clear();
        ui->username->setFocus();
        return;
    }

    // 暱稱校驗
    QRegularExpression regNickName(USER_REG);
    match = regNickName.match(nickname);
    if (!match.hasMatch()) {  // 檢查是否匹配成功
        QMessageBox::warning(this, "警告","暱稱格式不正確");
        ui->username->clear();
        ui->username->setFocus();
        return;
    }

    // 密碼校驗

    QRegularExpression regPwd(PASSWD_REG);
    match = regPwd.match(pwd);  // 使用 match 方法進行匹配
    if (!match.hasMatch()) {  // 檢查是否匹配成功
        QMessageBox::warning(this, "警告", "密碼格式不正確");
        ui->password->clear();
        ui->password->setFocus();
        return;
    }


    if(pwd != pwd2){
        QMessageBox::warning(this, "警告","兩次輸入的密碼不一致");
        ui->password->clear();
        ui->password->setFocus();
        return;
    }

    // phone校驗
    QRegularExpression phonePwd(PHONE_REG_MOBILE);
    match = phonePwd.match(phone);
    if (!match.hasMatch()) {  // 檢查是否匹配成功
        QMessageBox::warning(this, "警告","電話格式不正確");
        ui->password->clear();
        ui->password->setFocus();
        return;
    }

    // eamil校驗
    QRegularExpression emailPwd(EMAIL_REG);
    match = emailPwd.match(email);
    if (!match.hasMatch()) {  // 檢查是否匹配成功
        QMessageBox::warning(this, "警告","電子信箱格式不正確");
        ui->password->clear();
        ui->password->setFocus();
        return;
    }

    //1. 獲取輸入框的數據
    //2. 校驗數據
    //3. 發送http請求
    //  1. 設置url
    //  2. 設置標頭
    //  3. 封裝請求數據(json)
    //  4. 發送請求(get/post/put/delete)
    //  5. 讀取伺服器返回的數據
    //  6. 解析返回數據
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request; //棧
    //QNetworkRequest *request = new QNetworkRequest(); //堆
    QString ip = m_common->getConfValue("web_server", "ip");
    QString port = m_common->getConfValue("web_server", "port");
    QString url = QString("http://%1:%2/reg").arg(ip).arg(port);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    QJsonObject paramsObj;
    paramsObj.insert("email",email);
    paramsObj.insert("userName",username);
    paramsObj.insert("phone",phone);
    paramsObj.insert("nickName",nickname);
    paramsObj.insert("firstPwd",m_common->getStrMd5(pwd));
    QJsonDocument doc(paramsObj);

    QByteArray data = doc.toJson();
    QNetworkReply *reply = manager->post(request, data);

    // 讀取伺服器返回的數據
    connect(reply, &QNetworkReply::readyRead, this, [=](){
        // 讀數據
        QByteArray data = reply->readAll();
        qDebug() << "伺服器返回數據:" << QString(data);

        QJsonParseError err;
        // 解析返回數據
        QJsonDocument rootDoc = QJsonDocument::fromJson(data, &err);
        if(err.error != QJsonParseError::NoError){
            qDebug() << "Json格式錯誤";
        }else{
            // 解析json
            // {\n\t\"code\":\t\"002\"\n}

            QJsonObject rootObj = rootDoc.object();
            QJsonValue codeValue = rootObj.value("code");
            if(codeValue.type() == QJsonValue::String){
                QString code = codeValue.toString();
                //qDebug() << "code:" << codeValue.toString();
                if(code == "002"){
                    QMessageBox::information(this, "提示", "註冊成功");
                }else if(code == "003"){
                    QMessageBox::information(this, "提示", "該用戶已存在");
                }else if(code == "004"){
                    QMessageBox::critical(this, "錯誤", "註冊失敗");
                }
            }
        }
    });

}






void LoginDialog::on_set_ok_clicked()
{
    //要处理json
    QString ip = ui->server_ip->text();
    QString port = ui->server_port->text();

    // 数据判断
    // 服务器IP
    // \\d 和 \\. 中第一个\是转义字符, 这里使用的是标准正则
    QRegularExpression regexp(IP_REG);
    QRegularExpressionMatch match = regexp.match(ip);
    if (!match.hasMatch())   // 檢查是否匹配成功
    {
        QMessageBox::warning(this, "警告", "您输入的IP格式不正确, 请重新输入!");
        return;
    }
    // 端口号
    regexp.setPattern(PORT_REG);
    match = regexp.match(port);
    if (!match.hasMatch())
    {
        QMessageBox::warning(this, "警告", "您输入的端口格式不正确, 请重新输入!");
        return;
    }
    // 跳转到登陆界面
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    // 将配置信息写入配置文件中
    m_common->writeWebInfo(ip, port);
}


