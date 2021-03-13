#include "ProzorAnaliza.hpp"
#include "ui_ProzorAnaliza.h"

WiFiAnaliza::WiFiAnaliza(QWidget *parent) : QMainWindow(parent), ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);

    auto cvorovi = Citac::DohvatiSveCvorove();
    auto *ModelCvorovi = new CvorModel(this);
    ModelCvorovi->populateData(cvorovi);

    ui->qListViewCvorovi->setModel(ModelCvorovi);
    ui->qListViewCvorovi->show();
}

WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
}

void WiFiAnaliza::PostaviSucelje(std::string _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}
