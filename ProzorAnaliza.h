#ifndef PROZORANALIZA_H
#define PROZORANALIZA_H

#include <QAbstractTableModel>
#include <QMainWindow>
#include "Cvor.h"
#include "Citac.h"
#include "Okvir.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class WiFiAnaliza;
}
QT_END_NAMESPACE

class WiFiAnaliza : public QMainWindow
{
    Q_OBJECT

public:
    explicit WiFiAnaliza(QWidget *parent = nullptr);
    ~WiFiAnaliza();
    void PostaviSucelje(std::string);

public slots:
    void dodajOkvir(Okvir okvir);

private:
    Ui::WiFiAnaliza *ui;
    std::string nazivSucelja;
};

#endif // PROZORANALIZA_H


