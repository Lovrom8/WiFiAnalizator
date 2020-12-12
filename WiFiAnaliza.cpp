#include "WiFiAnaliza.h"
#include "ui_WiFiAnaliza.h"

WiFiAnaliza::WiFiAnaliza(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);
}

WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
}

void WiFiAnaliza::PostaviSucelje(std::string _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}
