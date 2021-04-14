#include "Cvor.h"

CvorModel::CvorModel(QObject *parent) : QAbstractTableModel(parent) {}

void CvorModel::populateData(QList<Cvor> _cvorovi){
    cvorovi.clear();
    cvorovi = _cvorovi;
}

int CvorModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return cvorovi.length();
}


int CvorModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant CvorModel::data(const QModelIndex &index, int role) const{
      if(!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

      if(index.column() == 0)
          return cvorovi[index.row()].MAC_Adresa;

      return QVariant();
}

QVariant CvorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if(section == 0)
        return QString("Čvor");

    return QVariant();
}

bool CvorModel::dodajCvor(Cvor cvor, const QModelIndex &parent)
{
    beginInsertRows(parent, cvorovi.count(), cvorovi.count());
    cvorovi.append(cvor);
    endInsertRows();
    return true;
}


