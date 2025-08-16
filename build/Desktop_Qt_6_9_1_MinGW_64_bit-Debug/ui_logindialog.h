/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "titlewidget.h"

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    TitleWidget *title_widget;
    QHBoxLayout *horizontalLayout_5;
    QStackedWidget *stackedWidget;
    QWidget *login_page;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QLabel *label_4;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *password;
    QLineEdit *username;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QCheckBox *chk_remember;
    QToolButton *notReg;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QToolButton *login;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QWidget *reg_page;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_5;
    QLabel *label;
    QWidget *widget_8;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QLineEdit *reg_username;
    QLabel *label_5;
    QLineEdit *reg_pwd;
    QLineEdit *reg_nickname;
    QLabel *label_8;
    QLabel *label_7;
    QLineEdit *reg_phone;
    QLabel *label_9;
    QLineEdit *reg_email;
    QLabel *label_10;
    QLineEdit *reg_pwd2;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QToolButton *btn_reg;
    QSpacerItem *horizontalSpacer_8;
    QWidget *set_page;
    QWidget *widget_10;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_9;
    QWidget *widget_12;
    QGridLayout *gridLayout_3;
    QLineEdit *server_port;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_12;
    QLineEdit *server_ip;
    QWidget *widget_11;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_9;
    QToolButton *set_ok;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_7;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(650, 443);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        title_widget = new TitleWidget(LoginDialog);
        title_widget->setObjectName("title_widget");
        horizontalLayout_5 = new QHBoxLayout(title_widget);
        horizontalLayout_5->setObjectName("horizontalLayout_5");

        verticalLayout->addWidget(title_widget);

        stackedWidget = new QStackedWidget(LoginDialog);
        stackedWidget->setObjectName("stackedWidget");
        login_page = new QWidget();
        login_page->setObjectName("login_page");
        horizontalLayout_3 = new QHBoxLayout(login_page);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(156, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        widget = new QWidget(login_page);
        widget->setObjectName("widget");
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        QFont font;
        font.setFamilies({QString::fromUtf8("Rockwell Extra Bold")});
        font.setPointSize(18);
        font.setBold(true);
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label_4);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(widget_2);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        password = new QLineEdit(widget_2);
        password->setObjectName("password");
        password->setMaximumSize(QSize(200, 16777215));
        password->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(password, 2, 1, 1, 1);

        username = new QLineEdit(widget_2);
        username->setObjectName("username");
        username->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(username, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 1, 1, 1, 1);


        verticalLayout_3->addWidget(widget_2);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName("horizontalLayout");
        chk_remember = new QCheckBox(widget_3);
        chk_remember->setObjectName("chk_remember");

        horizontalLayout->addWidget(chk_remember);

        notReg = new QToolButton(widget_3);
        notReg->setObjectName("notReg");
        notReg->setStyleSheet(QString::fromUtf8("color: rgb(255, 66, 29);\n"
"text-decoration: underline;\n"
"font: 10pt \"Agency FB\";\n"
""));
        notReg->setAutoRaise(true);

        horizontalLayout->addWidget(notReg);


        verticalLayout_3->addWidget(widget_3);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName("widget_4");
        horizontalLayout_2 = new QHBoxLayout(widget_4);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        login = new QToolButton(widget_4);
        login->setObjectName("login");
        login->setMinimumSize(QSize(130, 35));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Rockwell Extra Bold")});
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        login->setFont(font1);
        login->setStyleSheet(QString::fromUtf8("border-image: url(:/images/balckButton.png);\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(login);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(widget_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_3->addWidget(widget);

        horizontalSpacer_4 = new QSpacerItem(156, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(login_page);
        reg_page = new QWidget();
        reg_page->setObjectName("reg_page");
        horizontalLayout_4 = new QHBoxLayout(reg_page);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        widget_5 = new QWidget(reg_page);
        widget_5->setObjectName("widget_5");
        widget_5->setMinimumSize(QSize(0, 300));
        widget_5->setMaximumSize(QSize(300, 300));
        label = new QLabel(widget_5);
        label->setObjectName("label");
        label->setGeometry(QRect(90, 0, 108, 31));
        label->setMaximumSize(QSize(16777215, 100));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Broadway")});
        font2.setPointSize(18);
        label->setFont(font2);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        widget_8 = new QWidget(widget_5);
        widget_8->setObjectName("widget_8");
        widget_8->setGeometry(QRect(9, 46, 251, 168));
        gridLayout_2 = new QGridLayout(widget_8);
        gridLayout_2->setObjectName("gridLayout_2");
        label_6 = new QLabel(widget_8);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        reg_username = new QLineEdit(widget_8);
        reg_username->setObjectName("reg_username");
        reg_username->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(reg_username, 0, 2, 1, 1);

        label_5 = new QLabel(widget_8);
        label_5->setObjectName("label_5");

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        reg_pwd = new QLineEdit(widget_8);
        reg_pwd->setObjectName("reg_pwd");
        reg_pwd->setMaximumSize(QSize(200, 16777215));
        reg_pwd->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_2->addWidget(reg_pwd, 2, 2, 1, 1);

        reg_nickname = new QLineEdit(widget_8);
        reg_nickname->setObjectName("reg_nickname");
        reg_nickname->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(reg_nickname, 1, 2, 1, 1);

        label_8 = new QLabel(widget_8);
        label_8->setObjectName("label_8");

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        label_7 = new QLabel(widget_8);
        label_7->setObjectName("label_7");

        gridLayout_2->addWidget(label_7, 3, 0, 1, 2);

        reg_phone = new QLineEdit(widget_8);
        reg_phone->setObjectName("reg_phone");
        reg_phone->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(reg_phone, 4, 2, 1, 1);

        label_9 = new QLabel(widget_8);
        label_9->setObjectName("label_9");

        gridLayout_2->addWidget(label_9, 4, 0, 1, 1);

        reg_email = new QLineEdit(widget_8);
        reg_email->setObjectName("reg_email");
        reg_email->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(reg_email, 5, 2, 1, 1);

        label_10 = new QLabel(widget_8);
        label_10->setObjectName("label_10");

        gridLayout_2->addWidget(label_10, 5, 0, 1, 1);

        reg_pwd2 = new QLineEdit(widget_8);
        reg_pwd2->setObjectName("reg_pwd2");
        reg_pwd2->setMaximumSize(QSize(200, 16777215));
        reg_pwd2->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_2->addWidget(reg_pwd2, 3, 2, 1, 1);

        widget_9 = new QWidget(widget_5);
        widget_9->setObjectName("widget_9");
        widget_9->setGeometry(QRect(9, 220, 282, 53));
        horizontalLayout_6 = new QHBoxLayout(widget_9);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer_7 = new QSpacerItem(89, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        btn_reg = new QToolButton(widget_9);
        btn_reg->setObjectName("btn_reg");
        btn_reg->setMinimumSize(QSize(130, 35));
        btn_reg->setFont(font1);
        btn_reg->setStyleSheet(QString::fromUtf8("border-image: url(:/images/balckButton.png);\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_6->addWidget(btn_reg);

        horizontalSpacer_8 = new QSpacerItem(89, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);


        horizontalLayout_4->addWidget(widget_5);

        stackedWidget->addWidget(reg_page);
        set_page = new QWidget();
        set_page->setObjectName("set_page");
        widget_10 = new QWidget(set_page);
        widget_10->setObjectName("widget_10");
        widget_10->setGeometry(QRect(160, 10, 284, 361));
        verticalLayout_5 = new QVBoxLayout(widget_10);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalSpacer_6 = new QSpacerItem(20, 72, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_6);

        label_11 = new QLabel(widget_10);
        label_11->setObjectName("label_11");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Berlin Sans FB Demi")});
        font3.setPointSize(18);
        font3.setBold(true);
        label_11->setFont(font3);
        label_11->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_5->addWidget(label_11);

        verticalSpacer_9 = new QSpacerItem(20, 50, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_9);

        widget_12 = new QWidget(widget_10);
        widget_12->setObjectName("widget_12");
        gridLayout_3 = new QGridLayout(widget_12);
        gridLayout_3->setObjectName("gridLayout_3");
        server_port = new QLineEdit(widget_12);
        server_port->setObjectName("server_port");
        server_port->setMaximumSize(QSize(200, 16777215));

        gridLayout_3->addWidget(server_port, 4, 1, 1, 1);

        label_13 = new QLabel(widget_12);
        label_13->setObjectName("label_13");

        gridLayout_3->addWidget(label_13, 4, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_5, 3, 1, 1, 1);

        label_12 = new QLabel(widget_12);
        label_12->setObjectName("label_12");

        gridLayout_3->addWidget(label_12, 2, 0, 1, 1);

        server_ip = new QLineEdit(widget_12);
        server_ip->setObjectName("server_ip");
        server_ip->setMaximumSize(QSize(200, 16777215));

        gridLayout_3->addWidget(server_ip, 2, 1, 1, 1);


        verticalLayout_5->addWidget(widget_12);

        widget_11 = new QWidget(widget_10);
        widget_11->setObjectName("widget_11");
        horizontalLayout_7 = new QHBoxLayout(widget_11);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_9);

        set_ok = new QToolButton(widget_11);
        set_ok->setObjectName("set_ok");
        set_ok->setMinimumSize(QSize(120, 35));
        set_ok->setFont(font1);
        set_ok->setStyleSheet(QString::fromUtf8("border-image: url(:/images/balckButton.png);\n"
"color: rgb(255, 255, 255);"));

        horizontalLayout_7->addWidget(set_ok);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_10);


        verticalLayout_5->addWidget(widget_11);

        verticalSpacer_7 = new QSpacerItem(20, 72, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_7);

        stackedWidget->addWidget(set_page);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(LoginDialog);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\266\347\231\273\345\205\245", nullptr));
        label_2->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\266\345\220\215", nullptr));
        label_3->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\242\274", nullptr));
        chk_remember->setText(QCoreApplication::translate("LoginDialog", "\350\250\230\344\275\217\345\257\206\347\242\274", nullptr));
        notReg->setText(QCoreApplication::translate("LoginDialog", "\346\262\222\346\234\211\345\270\263\350\231\237\357\274\214\347\253\213\345\215\263\350\250\273\345\206\212", nullptr));
        login->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\205\245", nullptr));
        label->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\266\350\250\273\345\206\212", nullptr));
        label_6->setText(QCoreApplication::translate("LoginDialog", "\346\232\261\347\250\261", nullptr));
        label_5->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\266\345\220\215", nullptr));
        label_8->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\242\274", nullptr));
        label_7->setText(QCoreApplication::translate("LoginDialog", "\347\242\272\350\252\215\345\257\206\347\242\274", nullptr));
        reg_phone->setText(QString());
        label_9->setText(QCoreApplication::translate("LoginDialog", "\346\211\213\346\251\237", nullptr));
        reg_email->setText(QString());
        label_10->setText(QCoreApplication::translate("LoginDialog", "\351\203\265\347\256\261", nullptr));
        btn_reg->setText(QCoreApplication::translate("LoginDialog", "\350\250\273\345\206\212", nullptr));
        label_11->setText(QCoreApplication::translate("LoginDialog", "\344\274\272\346\234\215\345\231\250\350\250\255\347\275\256", nullptr));
        label_13->setText(QCoreApplication::translate("LoginDialog", "\344\274\272\346\234\215\345\231\250\345\237\240\350\231\237:", nullptr));
        label_12->setText(QCoreApplication::translate("LoginDialog", "\344\274\272\346\234\215\345\231\250\345\234\260\345\235\200:", nullptr));
        set_ok->setText(QCoreApplication::translate("LoginDialog", "\347\242\272\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
