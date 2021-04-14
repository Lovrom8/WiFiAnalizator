#include "ProzorAnaliza.h"
#include "ui_ProzorAnaliza.h"

OkvirModel *ModelOkviri;

WiFiAnaliza::WiFiAnaliza(QWidget *parent) : QMainWindow(parent), ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);

    Citac* citac = new Citac();
    connect(citac, SIGNAL(noviOkvir(Okvir)), this, SLOT(dodajOkvir(Okvir)));
    citac->pokreni();

     citac->PokreniCitanjePrometa("wlx00c0caac4467");

    //auto cvorovi = Citac::DohvatiSveCvorove();
   // auto *ModelCvorovi = new CvorModel(this);
    //ModelCvorovi->populateData(cvorovi);

    //ui->qListViewCvorovi->setModel(ModelCvorovi);
    //ui->qListViewCvorovi->show();

    //ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(cvorovi.length()));

    QList<Okvir> okviri;
    Okvir testni;
    testni.Vrijeme = 1;
    testni.VrstaOkvira = "testna";
    okviri.append(testni);
    ModelOkviri = new OkvirModel(this);
    ModelOkviri->populateData(okviri);

    ui->tableOkviri->setModel(ModelOkviri);
    ui->tableOkviri->show();

    testni.VrstaOkvira = "testet";
    ModelOkviri->dodajOkvir(testni);
}

WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
}

void WiFiAnaliza::PostaviSucelje(std::string _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}

void WiFiAnaliza::dodajOkvir(Okvir okvir) {
    //qDebug() <<  okvir.VrstaOkvira.c_str() << " poslano natrag";

    if(ModelOkviri != NULL)
     ModelOkviri->dodajOkvir(okvir);
}
