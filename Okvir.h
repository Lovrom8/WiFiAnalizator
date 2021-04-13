#pragma once
#include <string>
#include <QAbstractTableModel>

struct MAC_Adr{
    std::string Adr1;
    std::string Adr2;
    std::string Adr3;
    std::string Adr4;
};

class Okvir{
public:
    double Vrijeme;
    MAC_Adr MAC;
    int JacinaSignala;
    std::string VrstaOkvira;
};

class OkvirModel : public QAbstractTableModel {
    Q_OBJECT

public:
       OkvirModel(QObject *parent = NULL);

       void populateData(QList<Okvir> okviri);

       int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
       int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

       QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
       QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
       QList<Okvir> okviri;
};
