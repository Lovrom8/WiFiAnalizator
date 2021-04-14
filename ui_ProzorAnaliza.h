/********************************************************************************
** Form generated from reading UI file 'ProzorAnaliza.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROZORANALIZA_H
#define UI_PROZORANALIZA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WiFiAnaliza
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QListView *qListViewCvorovi;
    QLabel *labelAktivnihCvorova;
    QTableView *tableOkviri;
    QLabel *label_2;
    QMenuBar *menubar;
    QMenu *menuWiFi_Analiza;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WiFiAnaliza)
    {
        if (WiFiAnaliza->objectName().isEmpty())
            WiFiAnaliza->setObjectName(QStringLiteral("WiFiAnaliza"));
        WiFiAnaliza->resize(677, 494);
        centralwidget = new QWidget(WiFiAnaliza);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 10, 67, 19));
        qListViewCvorovi = new QListView(centralwidget);
        qListViewCvorovi->setObjectName(QStringLiteral("qListViewCvorovi"));
        qListViewCvorovi->setGeometry(QRect(30, 30, 341, 91));
        labelAktivnihCvorova = new QLabel(centralwidget);
        labelAktivnihCvorova->setObjectName(QStringLiteral("labelAktivnihCvorova"));
        labelAktivnihCvorova->setGeometry(QRect(390, 40, 141, 41));
        tableOkviri = new QTableView(centralwidget);
        tableOkviri->setObjectName(QStringLiteral("tableOkviri"));
        tableOkviri->setGeometry(QRect(30, 180, 631, 271));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 160, 67, 19));
        WiFiAnaliza->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WiFiAnaliza);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 677, 24));
        menuWiFi_Analiza = new QMenu(menubar);
        menuWiFi_Analiza->setObjectName(QStringLiteral("menuWiFi_Analiza"));
        WiFiAnaliza->setMenuBar(menubar);
        statusbar = new QStatusBar(WiFiAnaliza);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        WiFiAnaliza->setStatusBar(statusbar);

        menubar->addAction(menuWiFi_Analiza->menuAction());

        retranslateUi(WiFiAnaliza);

        QMetaObject::connectSlotsByName(WiFiAnaliza);
    } // setupUi

    void retranslateUi(QMainWindow *WiFiAnaliza)
    {
        WiFiAnaliza->setWindowTitle(QApplication::translate("WiFiAnaliza", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("WiFiAnaliza", "\304\214vorovi", Q_NULLPTR));
        labelAktivnihCvorova->setText(QApplication::translate("WiFiAnaliza", "Aktivnih \304\215vorova:", Q_NULLPTR));
        label_2->setText(QApplication::translate("WiFiAnaliza", "Paketi", Q_NULLPTR));
        menuWiFi_Analiza->setTitle(QApplication::translate("WiFiAnaliza", "WiFi Analiza", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WiFiAnaliza: public Ui_WiFiAnaliza {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROZORANALIZA_H
