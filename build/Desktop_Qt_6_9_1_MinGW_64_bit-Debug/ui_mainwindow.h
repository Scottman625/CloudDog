/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <downloadwidget.h>
#include <sharewidget.h>
#include <transformwidget.h>
#include "buttongroup.h"
#include "myfilewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    ButtonGroup *button_group;
    QStackedWidget *stackedWidget;
    MyFileWidget *myFile_page;
    ShareWidget *share_page;
    DownloadWidget *download_page;
    TransformWidget *transform_page;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        button_group = new ButtonGroup(centralWidget);
        button_group->setObjectName("button_group");

        verticalLayout->addWidget(button_group);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName("stackedWidget");
        myFile_page = new MyFileWidget();
        myFile_page->setObjectName("myFile_page");
        stackedWidget->addWidget(myFile_page);
        share_page = new ShareWidget();
        share_page->setObjectName("share_page");
        stackedWidget->addWidget(share_page);
        download_page = new DownloadWidget();
        download_page->setObjectName("download_page");
        stackedWidget->addWidget(download_page);
        transform_page = new TransformWidget();
        transform_page->setObjectName("transform_page");
        stackedWidget->addWidget(transform_page);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
