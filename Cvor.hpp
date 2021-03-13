#pragma once
#include <QAbstractTableModel>
#include <string>

class Cvor {
public:
     std::string MAC_Adresa;
};

class CvorModel : public QAbstractTableModel {
    Q_OBJECT

public:
       CvorModel(QObject *parent = NULL);

       void populateData(QList<Cvor> cvorovi);

       int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
       int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

       QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
       QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
       QList<Cvor> cvorovi;
};
