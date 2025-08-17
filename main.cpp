#include "mainwindow.h"
#include <QApplication>
#include <logindialog.h>
#include <QTextCodec>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 設定字符編碼以解決中文亂碼問題
    #if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    #else
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    #endif
    
    // 設定控制台輸出編碼
    #ifdef Q_OS_WIN
        SetConsoleOutputCP(CP_UTF8);
    #endif
    
//    MainWindow w;
//    w.show();

    LoginDialog lg;
    lg.show();

    return a.exec();
}
