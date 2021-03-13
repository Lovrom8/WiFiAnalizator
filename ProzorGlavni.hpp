#ifndef PROZORGLAVNI_H
#define PROZORGLAVNI_H

#include <QMainWindow>
#include "WiFiSucelja.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class GlavniProzor; }
QT_END_NAMESPACE

class GlavniProzor : public QMainWindow
{
    Q_OBJECT

public:
    GlavniProzor(QWidget *parent = nullptr);
    ~GlavniProzor();

private:
    Ui::GlavniProzor *ui;
    void PopuniWifiSucelja();
    void StaviUMonitorski_Klik();
};
#endif // PROZORGLAVNI_H
