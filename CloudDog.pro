#-------------------------------------------------
#
# Project created by QtCreator 2025-01-27T16:08:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudDog
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# 設定UTF-8編碼以解決中文亂碼問題
DEFINES += UNICODE
DEFINES += _UNICODE

# MinGW編譯器編碼設定
mingw {
    QMAKE_CXXFLAGS += -finput-charset=UTF-8
    QMAKE_CXXFLAGS += -fexec-charset=UTF-8
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    buttongroup.cpp \
    downloadlayout.cpp \
    downloadtask.cpp \
    downloadwidget.cpp \
    filedataprogress.cpp \
    filepropertyinfodialog.cpp \
    logininfoinstance.cpp \
        main.cpp \
        mainwindow.cpp \
    logindialog.cpp \
    myfilewidget.cpp \
    mymenu.cpp \
    networkdata.cpp \
    sharewidget.cpp \
    titlewidget.cpp \
    des.c \
    common.cpp \
    transformwidget.cpp \
    uploadlayout.cpp \
    uploadtask.cpp

HEADERS += \
    buttongroup.h \
    downloadlayout.h \
    downloadtask.h \
    downloadwidget.h \
    filedataprogress.h \
    fileinfo.h \
    filepropertyinfodialog.h \
    logininfoinstance.h \
        mainwindow.h \
    logindialog.h \
    myfilewidget.h \
    mymenu.h \
    networkdata.h \
    sharewidget.h \
    titlewidget.h \
    common.h \
    des.h \
    transformwidget.h \
    uploadlayout.h \
    uploadtask.h

FORMS += \
    buttongroup.ui \
    downloadwidget.ui \
    filedataprogress.ui \
    filepropertyinfodialog.ui \
    logindialog.ui \
    mainwindow.ui \
    myfilewidget.ui \
    sharewidget.ui \
    titlewidget.ui \
    transformwidget.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    conf/cfg.json

