#include "WiFiSucelja.h"

namespace WifiSucelja {
    bool JeBezicni(QString naziv){
        int sockfd = -1;

        //auto *pwrq = new iwreq();
        //strncpy(pwrq->ifr_ifrn.ifrn_name, naziv.c_str(), IFNAMSIZ);

        iwreq pwrq;
        strncpy(pwrq.ifr_ifrn.ifrn_name, naziv.toStdString().c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        if(ioctl(sockfd, SIOCGIWNAME, &pwrq) != -1){
            close(sockfd);
            return true;
        }

        close(sockfd);
        return false;
    }

    QList<QString> GetSvaSucelja(){
        QList<QString> nazivi;

        ifaddrs *ifaddr, *_addr;

        if(getifaddrs(&ifaddr) == -1) {
            return nazivi;
        }

        _addr = ifaddr;
        while (_addr != NULL) {
            if(_addr->ifa_addr != NULL && _addr->ifa_addr->sa_family == AF_PACKET) {
                if(JeBezicni(_addr->ifa_name))
                    nazivi.push_back(_addr->ifa_name);
            }

            _addr = _addr->ifa_next;
        }

        freeifaddrs(ifaddr);
        return nazivi;
    }

    static int PostaviZastavice(QString naziv, int flags){
        int sockfd = -1;

        struct ifreq ifr;
        strncpy(ifr.ifr_ifrn.ifrn_name, naziv.toStdString().c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        ifr.ifr_flags = flags & 0xffff;

        if(ioctl(sockfd, SIOCSIFFLAGS, (caddr_t) &ifr) != -1){
            close(sockfd);
            return true;
        }

        close(sockfd);
        return false;
    }

    int UzmiZastavice(QString naziv, int* flags){
        int sockfd = -1;

        struct ifreq ifr;
        strncpy(ifr.ifr_ifrn.ifrn_name, naziv.toStdString().c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        if(ioctl(sockfd, SIOCGIFFLAGS, (caddr_t) &ifr) != -1){
            *flags = ifr.ifr_ifru.ifru_flags & 0xffff;
        }

        close(sockfd);
        return true;
    }

    void UgasiSucelje(QString uredaj){
        int zastavice;
        UzmiZastavice(uredaj, &zastavice);

        if(zastavice != 0x1003)
            return;

        qDebug() << "Gasim sucelje, zastavice: " << zastavice;
        PostaviZastavice(uredaj, ~zastavice);
    }

    void UpaliSucelje(QString uredaj){
        int zastavice;
        UzmiZastavice(uredaj, &zastavice);

        if(zastavice == 0x1003)
            return;

        qDebug() << "Palim sucelje, zastavice: " << zastavice;
        PostaviZastavice(uredaj, 0x1003);
    }

    bool Postavi(QString uredaj){
        int sockfd = -1;

        iwreq wrq;
        strncpy(wrq.ifr_ifrn.ifrn_name, uredaj.toStdString().c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        wrq.u.mode = 6; // 6 je monitorski način, 0 = auto, 1 = managed
        if(ioctl(sockfd, SIOCSIWMODE, &wrq) != -1){
            qDebug() << "Uspješno promijenjen način rada sučelja";
            close(sockfd);
            return true;
        }else{
            qDebug() << strerror(errno);
            close(sockfd);
            return false;
        }
    }

    bool PostaviUMonitorskiNacin(QString uredaj){
        UgasiSucelje(uredaj);
        bool uspjeh = Postavi(uredaj);
        UpaliSucelje(uredaj);

        return uspjeh;
    }
}

WifiSuceljeModel::WifiSuceljeModel(QObject *parent) : QAbstractTableModel(parent) {}

void WifiSuceljeModel::populateData(QList<QString> _sucelja){
    sucelja.clear();
    sucelja = _sucelja;
}

int WifiSuceljeModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return sucelja.length();
}


int WifiSuceljeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant WifiSuceljeModel::data(const QModelIndex &index, int role) const{
      if(!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

      if(index.column() == 0)
          return sucelja[index.row()];

      return QVariant();
}

QVariant WifiSuceljeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if(section == 0)
        return QString("Sucelje");

    return QVariant();
}
