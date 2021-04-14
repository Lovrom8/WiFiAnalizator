/********************************************************************************
** Form generated from reading UI file 'ProzorGlavni.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROZORGLAVNI_H
#define UI_PROZORGLAVNI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
            GlavniProzor->setObjectName(QStringLiteral("GlavniProzor"));
        GlavniProzor->resize(339, 305);
        centralwidget = new QWidget(GlavniProzor);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        btnPostaviUMonitorski = new QPushButton(centralwidget);
        btnPostaviUMonitorski->setObjectName(QStringLiteral("btnPostaviUMonitorski"));
        btnPostaviUMonitorski->setGeometry(QRect(60, 230, 201, 27));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 101, 19));
        qtTableSucelja = new QTableView(centralwidget);
        qtTableSucelja->setObjectName(QStringLiteral("qtTableSucelja"));
        qtTableSucelja->setGeometry(QRect(20, 30, 291, 192));
        qtTableSucelja->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        qtTableSucelja->horizontalHeader()->setCascadingSectionResizes(true);
        GlavniProzor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GlavniProzor);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 339, 24));
        menuWifiAnalizator = new QMenu(menubar);
        menuWifiAnalizator->setObjectName(QStringLiteral("menuWifiAnalizator"));
        GlavniProzor->setMenuBar(menubar);
        statusbar = new QStatusBar(GlavniProzor);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        GlavniProzor->setStatusBar(statusbar);

        menubar->addAction(menuWifiAnalizator->menuAction());

        retranslateUi(GlavniProzor);

        QMetaObject::connectSlotsByName(GlavniProzor);
    } // setupUi

    void retranslateUi(QMainWindow *GlavniProzor)
    {
        GlavniProzor->setWindowTitle(QApplication::translate("GlavniProzor", "WiFi Analizator", Q_NULLPTR));
        btnPostaviUMonitorski->setText(QApplication::translate("GlavniProzor", "Postavi u monitorski na\304\215in", Q_NULLPTR));
        label->setText(QApplication::translate("GlavniProzor", "Be\305\276i\304\215na su\304\215elja", Q_NULLPTR));
        menuWifiAnalizator->setTitle(QApplication::translate("GlavniProzor", "WifiAnalizator", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GlavniProzor: public Ui_GlavniProzor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROZORGLAVNI_H
