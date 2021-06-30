#ifndef PROZORANALIZA_H
#define PROZORANALIZA_H

#include <QAbstractTableModel>
#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QTimer>

#include "Cvor.h"
#include "Citac.h"
#include "Okvir.h"
#include "CvorPromet.h"

#include <map>


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
    void PostaviSucelje(QString);
    void PokreniCitanje(QString adapter);
    void StartStop_Klik();

signals:
    void osvjeziStatReq();

public slots:
    void dodajOkvir(Okvir okvir);
    void dodajCvor(Cvor cvor);
    void osvjeziStat(const std::vector<Okvir> &_okviri);
    void osjveziTimerStat();

private:
    Ui::WiFiAnaliza *ui;
    QString nazivSucelja;
    OkvirModel *ModelOkviri;
    CvorModel *ModelCvorovi;
    CvorPrometModel *ModelPromet;
    QList<Okvir> okviri;
    QList<Cvor> cvorovi;
    QList<CvorPromet> promet;
    QTimer *statTimer;

    QBarSet *setData;
    QBarSet *setMgmt;
    QBarSet *setControl;
    QChartView *chartView;
    QChart *chart;
    QBarSeries *series;

    QLineSeries *seriesJacine;
    QChartView *chartViewJacine;
    QChart *chartJacine;

    void OsvjeziGrafVrste(const std::vector<Okvir> &_okviri);
    void OsvjeziGrafJacine(const std::vector<Okvir> &_okviri);
};

#endif // PROZORANALIZA_H


