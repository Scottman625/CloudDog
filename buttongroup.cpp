#include "buttongroup.h"
#include "ui_buttongroup.h"
#include <QDebug>
#include <QPainter>
#include <QSignalMapper>

ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);

    //qDebug() << "parent:" << parent->objectName();

    //最小化
    QObject::connect(ui->btnMin, &QToolButton::clicked, this, [=](){
        emit minWindow();
    });

    //最大化
    QObject::connect(ui->btnMax, &QToolButton::clicked, this, [=](){
        static bool flag = false;
        if (!flag) {
            ui->btnMax->setIcon(QIcon(":/images/main/window/title_normal.png"));
        } else {
            ui->btnMax->setIcon(QIcon(":/images/main/window/title_max.png"));
        }
        flag = !flag;
        emit maxWindow();
    });

    //關閉
    QObject::connect(ui->btnClose, &QToolButton::clicked, this, [=](){
        emit closeWindow();
    });

    m_curBtn = ui->btnMyFile;
    defaultPage();

    //將一組由標識的發送者的signal連接在一起，通過setMapping來實現的
    //
    m_mapper = new QSignalMapper(this);

    m_map.insert(ui->btnMyFile->text(), ui->btnMyFile);
    m_map.insert(ui->btnShare->text(), ui->btnShare);
    m_map.insert(ui->btnDownload->text(), ui->btnDownload);
    m_map.insert(ui->btnTransform->text(), ui->btnTransform);

    qDebug() << "設置信號映射，按鈕文本:";
    QMap<QString, QToolButton*>::iterator iter = m_map.begin();
    for (; iter != m_map.end(); iter++) {
        qDebug() << "按鈕:" << iter.value()->objectName() << "文本:" << iter.key();
        m_mapper->setMapping(iter.value(), iter.key());
        connect(iter.value(), &QToolButton::clicked, m_mapper, QOverload<>::of(&QSignalMapper::map));
    }

    connect(m_mapper, QOverload<const QString &>::of(&QSignalMapper::mappedString), 
            this, &ButtonGroup::onMapperButtonsClicked);


/*
    connect(ui->btnMyFile, &QToolButton::clicked, this, [=](){
        emit sigMyFile();

        ui->btnMyFile->setStyleSheet("color:white");
        ui->btnShare->setStyleSheet("color:black");
        ui->btnDownload->setStyleSheet("color:black");
        ui->btnTransform->setStyleSheet("color:black");
        ui->btnSwitchUser->setStyleSheet("color:black");
    });
    connect(ui->btnShare, &QToolButton::clicked, this, [=](){
        emit sigShare();

        ui->btnMyFile->setStyleSheet("color:black");
        ui->btnShare->setStyleSheet("color:white");
        ui->btnDownload->setStyleSheet("color:black");
        ui->btnTransform->setStyleSheet("color:black");
        ui->btnSwitchUser->setStyleSheet("color:black");
    });
    connect(ui->btnDownload, &QToolButton::clicked, this, [=](){
        emit sigDownload();

        ui->btnMyFile->setStyleSheet("color:black");
        ui->btnShare->setStyleSheet("color:black");
        ui->btnDownload->setStyleSheet("color:white");
        ui->btnTransform->setStyleSheet("color:black");
        ui->btnSwitchUser->setStyleSheet("color:black");
    });
    connect(ui->btnTransform, &QToolButton::clicked, this, [=](){
        emit sigTransform();

        ui->btnMyFile->setStyleSheet("color:black");
        ui->btnShare->setStyleSheet("color:black");
        ui->btnDownload->setStyleSheet("color:black");
        ui->btnTransform->setStyleSheet("color:white");
        ui->btnSwitchUser->setStyleSheet("color:black");
    });
*/

    connect(ui->btnSwitchUser, &QToolButton::clicked, this, [=](){
        emit sigSwitchUser();

        defaultPage();
    });
}

void ButtonGroup::onMapperButtonsClicked(QString text) { //点击下载榜

    QToolButton* btnTemp = nullptr;  //选中按钮

    QMap<QString, QToolButton*>::iterator iter = m_map.begin();
    for (; iter != m_map.end(); iter++) {
        if (iter.key() == text) {
            btnTemp = (*iter);
            break;  // 找到後立即跳出循環
        }
    }

    if (btnTemp == nullptr) {
        qDebug() << "未找到按鈕:" << text;
        return;
    }

    if (btnTemp == m_curBtn) {
        qDebug() << "按鈕已經是當前選中狀態:" << text;
        return;
    }

    qDebug() << "切換按鈕從:" << m_curBtn->objectName() << "到:" << btnTemp->objectName();
    qDebug() << "按鈕文本:" << text;
    
    //1. 設定舊按鈕顏色為黑色
    m_curBtn->setStyleSheet("color:black");
    
    //2. 更新當前按鈕
    m_curBtn = btnTemp;
    
    //3. 設定新按鈕顏色為白色
    m_curBtn->setStyleSheet("color:white");

    //4. 發送信號
    if (text == ui->btnMyFile->text()) {
        qDebug() << "發送 sigMyFile 信號";
        emit sigMyFile();
    } else if (text == ui->btnShare->text()) {
        qDebug() << "發送 sigShare 信號";
        emit sigShare();
    } else if (text == ui->btnDownload->text()) {
        qDebug() << "發送 sigDownload 信號";
        emit sigDownload();
    } else if (text == ui->btnTransform->text()) {
        qDebug() << "發送 sigTransform 信號";
        emit sigTransform();
    } else {
        qDebug() << "未知按鈕文本:" << text;
    }
}

void ButtonGroup::defaultPage() {
    //默认是btnMyFile;
    m_curBtn->setStyleSheet("color:black");
    m_curBtn = ui->btnMyFile;
    ui->btnMyFile->setStyleSheet("color:white");
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    QPixmap bg(":/images/main/bk_top.png");
    p.drawPixmap(0,0,this->width(),this->height(), bg);
}

void ButtonGroup::setUser(QString user) {
    ui->btnUser->setText(user);
}

//方法
//函数
void ButtonGroup::setParent(QWidget *parent) {
    //m_parent是MainWindow
    m_parent = parent;
}

void ButtonGroup::mouseMoveEvent(QMouseEvent *event)
{
    //只允许鼠标左键拖动
    if (event->buttons() & Qt::LeftButton) {
        //窗口需要跟着鼠标移动
        //窗口左上角点 = 鼠标当前位置 - 差值
        m_parent->move(event->globalPos() - m_pt); //m_parent loginDialog移动
    }
}

void ButtonGroup::mousePressEvent(QMouseEvent *event) {
    //只允许鼠标左键按下
    if (event->buttons() == Qt::LeftButton) {
        //求差值 = 鼠标当前位置 - 窗口左上角位置
        m_pt = event->globalPos() - m_parent->geometry().topLeft(); //m_parent loginDialog左上角
    }
}
