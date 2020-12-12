#ifndef WIFIANALIZA_H
#define WIFIANALIZA_H

#include <QMainWindow>

namespace Ui {
class WiFiAnaliza;
}

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

#endif // WIFIANALIZA_H
