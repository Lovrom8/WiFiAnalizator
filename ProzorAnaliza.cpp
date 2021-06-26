#include "ProzorAnaliza.h"
#include "ui_ProzorAnaliza.h"

WiFiAnaliza::WiFiAnaliza(QWidget *parent) : QMainWindow(parent), ui(new Ui::WiFiAnaliza)
{
    ui->setupUi(this);

    /* TABLICA ČVOROVI */
    ModelCvorovi = new CvorModel(this);
    ModelCvorovi->populateData(cvorovi);

    ui->qListViewCvorovi->setModel(ModelCvorovi);
    ui->qListViewCvorovi->show();

    ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(cvorovi.length()));

    /* TABLICA OKVIRI */

    ModelOkviri = new OkvirModel(this);
    ModelOkviri->populateData(okviri);

    ui->tableOkviri->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableOkviri->setModel(ModelOkviri);
    ui->tableOkviri->show();

    /* TABLICA PROMET */

    ModelPromet = new CvorPrometModel(this);
    ModelPromet->populateData(promet);

    ui->tableBrojPaketa->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableBrojPaketa->setModel(ModelPromet);
    ui->tableBrojPaketa->show();

    /* OSTALO */
    connect(ui->btnStartStop, &QPushButton::released, this, &WiFiAnaliza::StartStop_Klik);

    /* STATISTIKA TIMER */
    statTimer = new QTimer(this);
    statTimer->setInterval(1000);
    statTimer->start();
    connect(statTimer, SIGNAL(timeout()), this, SLOT(osjveziTimerStat()));

    /* GRAFOVI */
    setData = new QBarSet("Data");
    setMgmt = new QBarSet("Management");
    setControl = new QBarSet("Control");

    series = new QBarSeries();
    series->append(setData);
    series->append(setMgmt);
    series->append(setControl);

    chart = new QChart();
    //chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Broj okvira po vrsti");

    QStringList categories;
    categories << "Broj paketa";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(600, 400);
    chartView->setParent(ui->groupBoxStatistika);
    chartView->move(35, 30);
}


WiFiAnaliza::~WiFiAnaliza()
{
    delete ui;
    //delete citac;
}

void WiFiAnaliza::osjveziTimerStat() {
        emit osvjeziStatReq();
}

void WiFiAnaliza::PokreniCitanje(QString adapter){
    Citac *citac = new Citac();

    citac->PokreniCitanjePrometa(adapter);

    connect(citac, SIGNAL(noviOkvir(Okvir)), this, SLOT(dodajOkvir(Okvir)), Qt::QueuedConnection);
    connect(citac, SIGNAL(noviCvor(Cvor)), this, SLOT(dodajCvor(Cvor)), Qt::QueuedConnection);
    connect(citac, SIGNAL(osvjeziStatResp(std::vector<Okvir>)), this, SLOT(osvjeziStat(std::vector<Okvir>)));
    connect(this, SIGNAL(osvjeziStatReq()), citac, SLOT(osvjeziStatCitac()), Qt::QueuedConnection);
}

void WiFiAnaliza::PostaviSucelje(QString _nazivSucelja){
    nazivSucelja = _nazivSucelja;
}

void WiFiAnaliza::dodajOkvir(Okvir okvir) {
    if(ModelOkviri != NULL)
     ModelOkviri->dodajOkvir(okvir);

    if(ModelPromet != NULL)
        ModelPromet->dodajPromet(okvir);
}

void WiFiAnaliza::OsvjeziGraf(const std::vector<Okvir> &_okviri) {
    int brojData =  std::count_if(std::begin(_okviri), std::end(_okviri), [](Okvir okvir) { return okvir.paket && okvir.paket->Vrsta == "Data"; });
    int brojMgmt = std::count_if(std::begin(_okviri), std::end(_okviri), [](Okvir okvir) { return okvir.paket && okvir.paket->Vrsta == "Management"; });
    int brojControl = std::count_if(std::begin(_okviri), std::end(_okviri), [](Okvir okvir) { return okvir.paket && okvir.paket->Vrsta == "Control"; });

    qDebug() << "lmao" << _okviri.size() << " " << brojData << " " << brojMgmt << " " << brojControl;

    series->remove(setData);
    series->remove(setMgmt);
    series->remove(setControl);

    setData = new QBarSet("Data");
    setMgmt = new QBarSet("Management");
    setControl = new QBarSet("Control");

    series->append(setData);
    series->append(setMgmt);
    series->append(setControl);
    chart->removeSeries( series );

    *setData << brojData;
    *setMgmt << brojMgmt;
    *setControl << brojControl;
    chart->addSeries( series );

    chart->axisY()->setRange(0, std::max({brojData, brojMgmt, brojControl}));
    chartView->update();
}

void WiFiAnaliza::OsvjeziPromet(const std::vector<Okvir> &_okviri) {
    std::map<QString, int> paketiPoCvoru;
    for(Okvir okvir : _okviri) {
        for(QString macAdresa :  okvir.macAdrese) {
            if(paketiPoCvoru.count(macAdresa))
            {
                paketiPoCvoru[macAdresa]++;
            }
            else
            {
                paketiPoCvoru[macAdresa] = 1;
            }
        }
    }
}

void WiFiAnaliza::osvjeziStat(const std::vector<Okvir> &_okviri) {
    OsvjeziGraf(_okviri);
    OsvjeziPromet(_okviri);
}

void WiFiAnaliza::dodajCvor(Cvor cvor)  {
   if(ModelCvorovi != NULL)
          ModelCvorovi->dodajCvor(cvor);

    ui->labelAktivnihCvorova->setText(QString("Aktivnih čvorova: ")+QString::number(ModelCvorovi->rowCount()));
}

void WiFiAnaliza::StartStop_Klik(){
    ui->btnStartStop->setText("Pauziraj");
}

