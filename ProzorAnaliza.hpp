#ifndef PROZORANALIZA_H
#define PROZORANALIZA_H

#include <QAbstractTableModel>
#include <QMainWindow>
#include "Cvor.hpp"
#include "Citac.hpp"

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

private:
    Ui::WiFiAnaliza *ui;
    std::string nazivSucelja;
};

#endif // PROZORANALIZA_H


