/********************************************************************************
** Form generated from reading UI file 'ProzorAnaliza.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROZORANALIZA_H
#define UI_PROZORANALIZA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WiFiAnaliza
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QListView *qListViewCvorovi;
    QListView *qLiist2;
    QMenuBar *menubar;
    QMenu *menuWiFi_Analiza;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WiFiAnaliza)
    {
        if (WiFiAnaliza->objectName().isEmpty())
            WiFiAnaliza->setObjectName(QString::fromUtf8("WiFiAnaliza"));
        WiFiAnaliza->resize(800, 600);
        centralwidget = new QWidget(WiFiAnaliza);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 10, 67, 19));
        qListViewCvorovi = new QListView(centralwidget);
        qListViewCvorovi->setObjectName(QString::fromUtf8("qListViewCvorovi"));
        qListViewCvorovi->setGeometry(QRect(30, 30, 341, 91));
        qLiist2 = new QListView(centralwidget);
        qLiist2->setObjectName(QString::fromUtf8("qLiist2"));
        qLiist2->setGeometry(QRect(40, 140, 256, 192));
        WiFiAnaliza->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WiFiAnaliza);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuWiFi_Analiza = new QMenu(menubar);
        menuWiFi_Analiza->setObjectName(QString::fromUtf8("menuWiFi_Analiza"));
        WiFiAnaliza->setMenuBar(menubar);
        statusbar = new QStatusBar(WiFiAnaliza);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WiFiAnaliza->setStatusBar(statusbar);

        menubar->addAction(menuWiFi_Analiza->menuAction());

        retranslateUi(WiFiAnaliza);

        QMetaObject::connectSlotsByName(WiFiAnaliza);
    } // setupUi

    void retranslateUi(QMainWindow *WiFiAnaliza)
    {
        WiFiAnaliza->setWindowTitle(QCoreApplication::translate("WiFiAnaliza", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("WiFiAnaliza", "\304\214vorovi", nullptr));
        menuWiFi_Analiza->setTitle(QCoreApplication::translate("WiFiAnaliza", "WiFi Analiza", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WiFiAnaliza: public Ui_WiFiAnaliza {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROZORANALIZA_H
