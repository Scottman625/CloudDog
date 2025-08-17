#include "mainwindow.h"
#include <QApplication>
#include <logindialog.h>
#include <QDebug>
#include <QProcess>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    // 設定環境變數以支援UTF-8
    qputenv("QT_LOGGING_RULES", "qt.qpa.*=false");
    qputenv("QT_LOGGING_TO_CONSOLE", "1");
    
    // 強制設定UTF-8編碼
    qputenv("QT_QPA_PLATFORM", "windows:fontengine=freetype");
    
    QApplication a(argc, argv);
    
    // 設定控制台輸出編碼 (Qt 6中不需要QTextCodec)
    #ifdef Q_OS_WIN
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        // 設定控制台代碼頁
        system("chcp 65001 > nul");
    #endif
    
//    MainWindow w;
//    w.show();

    LoginDialog lg;
    lg.show();

    return a.exec();
}
