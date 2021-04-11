#include "ProzorAnaliza.hpp"
#include "ui_ProzorAnaliza.h"

WiFiAnaliza::WiFiAnaliza(QWidget *parent) : QMainWindow(parent), ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);

     Citac::PokreniCitanjePrometa("wlx00c0caac4467");

    auto cvorovi = Citac::DohvatiSveCvorove();
    auto *ModelCvorovi = new CvorModel(this);
    ModelCvorovi->populateData(cvorovi);

    ui->qListViewCvorovi->setModel(ModelCvorovi);
    ui->qListViewCvorovi->show();

    ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(cvorovi.length()));

    //auto okviri =
    auto *ModelOkviri = new OkvirModel(this);
    //ModelOkviri->populateData(okviri);

    ui->tableOkviri->setModel(ModelOkviri);
    ui->tableOkviri->show();
}

WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
}

void WiFiAnaliza::PostaviSucelje(std::string _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}
