#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <QAbstractTableModel>

#include "sys/socket.h"
#include "linux/wireless.h"
#include <linux/if_tun.h>
#include <ifaddrs.h>
#include "string.h"
#include "unistd.h"
#include "sys/ioctl.h"

#include <sys/types.h>

#include "Cvor.h"
#include <QDebug>
#include <mutex>

namespace WifiSucelja {
    bool JeBezicni(std::string naziv);
    QList<QString> GetSvaSucelja();
    QList<Cvor> DohvatiCvorove();

    bool PostaviUMonitorskiNacin(std::string uredaj="");

    unsigned int if_nametoindex(const char *ifname);
}

class WifiSuceljeModel : public QAbstractTableModel {
    Q_OBJECT

public:
       WifiSuceljeModel(QObject *parent = NULL);

       //void populateData(std::vector<std::string> _sucelja);
       void populateData(QList<QString> _sucelja);

       int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
       int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

       QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
       QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
       QList<QString> sucelja;
};


