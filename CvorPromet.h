#pragma once

#include <QAbstractTableModel>
#include <QDebug>

#include <string>

#include <Okvir.h>

class CvorPromet {
public:
    QString MAC;
    int BrojPaketa = 0;
};

class CvorPrometModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CvorPrometModel(QObject *parent = NULL);
    void populateData(QList<CvorPromet> cvorovi);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool dodajPromet(Okvir okvir, const QModelIndex &parent = QModelIndex());
    void sortByColumn(int column, Qt::SortOrder order);

private:
    QList<CvorPromet> promet;
};

Q_DECLARE_METATYPE(std::vector<CvorPromet>);
