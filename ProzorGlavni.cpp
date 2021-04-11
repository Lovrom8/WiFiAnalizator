#include "ProzorGlavni.hpp"
#include "ProzorAnaliza.hpp"
#include "ui_ProzorGlavni.h"
#include "Citac.hpp"
#include "WiFiSucelja.hpp"

GlavniProzor::GlavniProzor(QWidget *parent) : QMainWindow(parent), ui(new Ui::GlavniProzor)
{
    ui->setupUi(this);

    auto sucelja = WifiSucelja::GetSvaSucelja();
    WifiSuceljeModel *ModelSucelja = new WifiSuceljeModel(this);
    ModelSucelja->populateData(sucelja);

    ui->qtTableSucelja->setModel(ModelSucelja);
    ui->qtTableSucelja->horizontalHeader()->setVisible(true);
    ui->qtTableSucelja->show();

    connect(ui->btnPostaviUMonitorski, &QPushButton::released, this, &GlavniProzor::StaviUMonitorski_Klik);

    WiFiAnaliza *analiza = new WiFiAnaliza();
    analiza->show();
    close();
}

GlavniProzor::~GlavniProzor()
{
    delete ui;
}

void GlavniProzor::StaviUMonitorski_Klik(){
    QItemSelectionModel *odabraniRed = ui->qtTableSucelja->selectionModel();
    QString odabranoSucelje = odabraniRed->selectedRows(0).value(0).data().toString();

    odabranoSucelje = "wlx00c0caac4467";
    /*if(odabranoSucelje.size() == 0)
            return;

    QMessageBox msgBox;
    msgBox.setText(QString("Želite li postaviti sučelje %1 u monitorski način?").arg(odabranoSucelje));
    QPushButton *btnYes = msgBox.addButton(tr("Da"), QMessageBox::YesRole);
    msgBox.addButton(tr("Ne"), QMessageBox::NoRole);*/
   // msgBox.exec();

    //QMessageBox::warning(this, tr("Greška"), QString::number(odabranoSucelje.size()));
   // if(msgBox.clickedButton() != btnYes )
    //    return;




    /* OVO */
    /* Citac::PokreniCitanjePrometa(odabranoSucelje.toStdString()); */






    /*if(!WifiSucelja::PostaviUMonitorskiNacin(odabranoSucelje.toStdString())){
        QMessageBox::warning(this, tr("Greška"),
                             tr("Nije bilo moguće staviti adapter u monitorski način!"));
        return;
    }else{
        QMessageBox::warning(this, tr("Uspjeph"),
                             tr("Adapter je postavljen u monitorski način rada!"));
    }*/

   // WiFiAnaliza *analiza = new WiFiAnaliza();
   // analiza->show();
   // close();
}

