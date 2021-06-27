#include "CvorPromet.h"

CvorPrometModel::CvorPrometModel(QObject *parent) : QAbstractTableModel(parent) {}

void CvorPrometModel::populateData(QList<CvorPromet> _promet){
    promet.clear();
    promet = _promet;
}

int CvorPrometModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return promet.length();
}


int CvorPrometModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant CvorPrometModel::data(const QModelIndex &index, int role) const{
      if(!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

      if(index.column() == 0)
          return promet[index.row()].MAC;
      else if(index.column() == 1)
          return promet[index.row()].BrojPaketa;

      return QVariant();
}

QVariant CvorPrometModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if(section == 0)
        return QString("Čvor (MAC)");
    else if(section == 1)
        return QString("Broj paketa");

    return QVariant();
}

bool CvorPrometModel::dodajPromet(Okvir okvir, const QModelIndex &parent)
{
    for(QString mac : okvir.macAdrese) {
        auto cvor = std::find_if(std::begin(promet), std::end(promet), [&](CvorPromet &cvorInfo) { return cvorInfo.MAC == mac; });
        if(cvor != promet.end()) {
            cvor->BrojPaketa++;
        }else{
            beginInsertRows(parent, promet.count(), promet.count());

            CvorPromet noviCvor;
            noviCvor.BrojPaketa = 1;
            noviCvor.MAC = mac;
            promet.append(noviCvor);

            endInsertRows();
        }
    }

    return true;
}
