/********************************************************************************
** Form generated from reading UI file 'titlewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TITLEWIDGET_H
#define UI_TITLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TitleWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *logo;
    QLabel *title;
    QSpacerItem *horizontalSpacer;
    QToolButton *set;
    QToolButton *min;
    QToolButton *close;

    void setupUi(QWidget *TitleWidget)
    {
        if (TitleWidget->objectName().isEmpty())
            TitleWidget->setObjectName("TitleWidget");
        TitleWidget->resize(650, 50);
        horizontalLayout = new QHBoxLayout(TitleWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        logo = new QLabel(TitleWidget);
        logo->setObjectName("logo");

        horizontalLayout->addWidget(logo);

        title = new QLabel(TitleWidget);
        title->setObjectName("title");
        title->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 87 14pt \"Arial Black\";"));

        horizontalLayout->addWidget(title);

        horizontalSpacer = new QSpacerItem(476, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        set = new QToolButton(TitleWidget);
        set->setObjectName("set");
        set->setEnabled(true);
        set->setMinimumSize(QSize(0, 0));
        set->setMaximumSize(QSize(16777215, 16777215));
        set->setStyleSheet(QString::fromUtf8("border-image: url(:/images/login_setting.png);"));

        horizontalLayout->addWidget(set);

        min = new QToolButton(TitleWidget);
        min->setObjectName("min");
        min->setMinimumSize(QSize(0, 0));
        min->setStyleSheet(QString::fromUtf8("border-image: url(:/images/login_min.png);"));

        horizontalLayout->addWidget(min);

        close = new QToolButton(TitleWidget);
        close->setObjectName("close");
        close->setMinimumSize(QSize(0, 0));
        close->setStyleSheet(QString::fromUtf8("border-image: url(:/images/login_close.png);"));

        horizontalLayout->addWidget(close);


        retranslateUi(TitleWidget);

        QMetaObject::connectSlotsByName(TitleWidget);
    } // setupUi

    void retranslateUi(QWidget *TitleWidget)
    {
        TitleWidget->setWindowTitle(QCoreApplication::translate("TitleWidget", "Form", nullptr));
        logo->setText(QString());
        title->setText(QCoreApplication::translate("TitleWidget", "\351\233\262\347\253\257\347\213\227", nullptr));
        set->setText(QString());
        min->setText(QString());
        close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TitleWidget: public Ui_TitleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TITLEWIDGET_H
