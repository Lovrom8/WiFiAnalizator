#include "ProzorAnaliza.h"
#include "ui_ProzorAnaliza.h"

WiFiAnaliza::WiFiAnaliza(QWidget *parent) : QMainWindow(parent), ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);

    ModelCvorovi = new CvorModel(this);
    ModelCvorovi->populateData(cvorovi);

    ui->qListViewCvorovi->setModel(ModelCvorovi);
    ui->qListViewCvorovi->show();

    ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(cvorovi.length()));

    ModelOkviri = new OkvirModel(this);
    ModelOkviri->populateData(okviri);

    ui->tableOkviri->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableOkviri->setModel(ModelOkviri);
    ui->tableOkviri->show();

    connect(ui->btnStartStop, &QPushButton::released, this, &WiFiAnaliza::StartStop_Klik);

}

WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
    //delete citac;
}

void WiFiAnaliza::PokreniCitanje(std::string adapter){
    Citac *citac = new Citac();

    citac->PokreniCitanjePrometa("wlx00c0caac4467");

    connect(citac, SIGNAL(noviOkvir(Okvir)), this, SLOT(dodajOkvir(Okvir)));
    connect(citac, SIGNAL(noviCvor(Cvor)), this, SLOT(dodajCvor(Cvor)));
}

void WiFiAnaliza::PostaviSucelje(std::string _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}

void WiFiAnaliza::dodajOkvir(Okvir okvir) {
    if(ModelOkviri != NULL)
     ModelOkviri->dodajOkvir(okvir);
}

void WiFiAnaliza::dodajCvor(Cvor cvor)  {
   if(ModelCvorovi != NULL)
          ModelCvorovi->dodajCvor(cvor);

    ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(ModelCvorovi->rowCount()));
}

void WiFiAnaliza::StartStop_Klik(){
    ui->btnStartStop->setText("Pauziraj");
}
