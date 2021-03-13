/********************************************************************************
** Form generated from reading UI file 'ProzorGlavni.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROZORGLAVNI_H
#define UI_PROZORGLAVNI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GlavniProzor
{
public:
    QWidget *centralwidget;
    QPushButton *btnPostaviUMonitorski;
    QLabel *label;
    QTableView *qtTableSucelja;
    QMenuBar *menubar;
    QMenu *menuWifiAnalizator;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GlavniProzor)
    {
        if (GlavniProzor->objectName().isEmpty())
            GlavniProzor->setObjectName(QString::fromUtf8("GlavniProzor"));
        GlavniProzor->resize(656, 335);
        centralwidget = new QWidget(GlavniProzor);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btnPostaviUMonitorski = new QPushButton(centralwidget);
        btnPostaviUMonitorski->setObjectName(QString::fromUtf8("btnPostaviUMonitorski"));
        btnPostaviUMonitorski->setGeometry(QRect(230, 240, 201, 27));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 101, 19));
        qtTableSucelja = new QTableView(centralwidget);
        qtTableSucelja->setObjectName(QString::fromUtf8("qtTableSucelja"));
        qtTableSucelja->setGeometry(QRect(20, 30, 621, 192));
        qtTableSucelja->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        qtTableSucelja->horizontalHeader()->setCascadingSectionResizes(true);
        GlavniProzor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GlavniProzor);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 656, 24));
        menuWifiAnalizator = new QMenu(menubar);
        menuWifiAnalizator->setObjectName(QString::fromUtf8("menuWifiAnalizator"));
        GlavniProzor->setMenuBar(menubar);
        statusbar = new QStatusBar(GlavniProzor);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        GlavniProzor->setStatusBar(statusbar);

        menubar->addAction(menuWifiAnalizator->menuAction());

        retranslateUi(GlavniProzor);

        QMetaObject::connectSlotsByName(GlavniProzor);
    } // setupUi

    void retranslateUi(QMainWindow *GlavniProzor)
    {
        GlavniProzor->setWindowTitle(QCoreApplication::translate("GlavniProzor", "WiFi Analizator", nullptr));
        btnPostaviUMonitorski->setText(QCoreApplication::translate("GlavniProzor", "Postavi u monitorski na\304\215in", nullptr));
        label->setText(QCoreApplication::translate("GlavniProzor", "Be\305\276i\304\215na su\304\215elja", nullptr));
        menuWifiAnalizator->setTitle(QCoreApplication::translate("GlavniProzor", "WifiAnalizator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GlavniProzor: public Ui_GlavniProzor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROZORGLAVNI_H
