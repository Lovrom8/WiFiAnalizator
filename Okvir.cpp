#include "Okvir.h"

OkvirModel::OkvirModel(QObject *parent) : QAbstractTableModel(parent) {}

void OkvirModel::populateData(QList<Okvir> _okviri){
    okviri.clear();
    okviri = _okviri;
}

int OkvirModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return okviri.length();
}


int OkvirModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant OkvirModel::data(const QModelIndex &index, int role) const{
      if(!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

      if(index.column() == 0)
          return okviri[index.row()].Vrijeme;
      else  if(index.column() == 1)
          return QString::fromStdString(okviri[index.row()].VrstaOkvira);

      return QVariant();
}

QVariant OkvirModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if(section == 0)
        return QString("Vrijeme");
    else if(section == 1)
        return QString("Vrsta okvira");

    return QVariant();
}

bool OkvirModel::dodajOkvir(Okvir okvir, const QModelIndex &parent)
{
    beginInsertRows(parent, okviri.count(), okviri.count());
    okviri.append(okvir);
    endInsertRows();
    return true;
}
