#include "WifiSucelja.hpp"
#include <QDebug>

namespace WifiSucelja {
    bool JeBezicni(std::string naziv){
        int sockfd = -1;

        //auto *pwrq = new iwreq();
        //strncpy(pwrq->ifr_ifrn.ifrn_name, naziv.c_str(), IFNAMSIZ);

        iwreq pwrq;
        strncpy(pwrq.ifr_ifrn.ifrn_name, naziv.c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        if(ioctl(sockfd, SIOCGIWNAME, &pwrq) != -1){
            //if(protocol)
            //    strncpy(protocol, pwrq->u.name, IFNAMSIZ);

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

    int error_handler(sockaddr_nl *nla, nlmsgerr *err, void *arg){
        //int *ret = (int*)arg;
        //*ret = err->error;
        return NL_STOP;
    }

    int finish_handler(nl_msg *msg, void *arg){
        //int *(ret) = (int*)arg;
        //*ret = 0;
        return NL_SKIP;
    }

    static int expectedId;
    static int IFTypeCallback(struct nl_msg* msg, void* arg)
    {
        qDebug() << "callback";
        struct nlmsghdr* ret_hdr = nlmsg_hdr(msg);
        struct nlattr *tb_msg[NL80211_ATTR_MAX + 1];

        if (ret_hdr->nlmsg_type != expectedId)
        {
            // what is this??
            qDebug() << "kaj";
            return NL_STOP;
        }

        struct genlmsghdr *gnlh = (struct genlmsghdr*) nlmsg_data(ret_hdr);

        nla_parse(tb_msg, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
                  genlmsg_attrlen(gnlh, 0), NULL);

        if (tb_msg[NL80211_ATTR_IFTYPE]) {
            int type = nla_get_u32(tb_msg[NL80211_ATTR_IFTYPE]);

            qDebug() << "Type: " << type;
        }

        return 0;
    }

    bool GetIFType(std::string uredaj){
        unsigned int ret = 0;

        nl_sock *socket = nl_socket_alloc();
        genl_connect(socket);
        expectedId = genl_ctrl_resolve(socket, "nl80211");
        nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, IFTypeCallback, NULL);

        nl_msg *msg = nlmsg_alloc();
        genlmsg_put(msg, 0, 0, expectedId, 0, 0, NL80211_CMD_GET_INTERFACE, 0);

        int name = if_nametoindex(uredaj.c_str());
        NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, name);

        ret = nl_send_auto(socket, msg);
        nl_recvmsgs_default(socket);

        nl_socket_free(socket);
        nlmsg_free(msg);
        return true;

        nla_put_failure:
            nl_socket_free(socket);
            nlmsg_free(msg);

        return false;
    }

    static int PostaviZastavice(std::string naziv, int flags){
        int sockfd = -1;

        struct ifreq ifr;
        strncpy(ifr.ifr_ifrn.ifrn_name, naziv.c_str(), IFNAMSIZ);

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

    int UzmiZastavice(std::string naziv, int* flags){
        int sockfd = -1;

        struct ifreq ifr;
        strncpy(ifr.ifr_ifrn.ifrn_name, naziv.c_str(), IFNAMSIZ);

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            return false;
        }

        if(ioctl(sockfd, SIOCGIFFLAGS, (caddr_t) &ifr) != -1){
            *flags = ifr.ifr_ifru.ifru_flags & 0xffff;
        }

        close(sockfd);
        return true;
    }

    void UgasiSucelje(std::string uredaj){
        int zastavice;
        UzmiZastavice(uredaj, &zastavice);

        if(zastavice != 0x1003)
            return;

        qDebug() << "Gasim sucelje, zastavice: " << zastavice;
        PostaviZastavice(uredaj, ~zastavice);
    }

    void UpaliSucelje(std::string uredaj){
        int zastavice;
        UzmiZastavice(uredaj, &zastavice);

        if(zastavice == 0x1003)
            return;

        qDebug() << "Palim sucelje, zastavice: " << zastavice;
        PostaviZastavice(uredaj, 0x1003);
    }

    bool Postavi(std::string uredaj){
        int sockfd = -1;

        iwreq wrq;
        strncpy(wrq.ifr_ifrn.ifrn_name, uredaj.c_str(), IFNAMSIZ);

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

    bool PostaviUMonitorskiNLink(std::string uredaj){
        unsigned int ret = 0;

        nl_sock *socket = nl_socket_alloc();
        genl_connect(socket);
        expectedId = genl_ctrl_resolve(socket, "nl80211");
        nl_socket_modify_cb(socket, NL_CB_VALID, NL_CB_CUSTOM, IFTypeCallback, NULL);

        nl_msg *msg = nlmsg_alloc();
        genlmsg_put(msg, 0, 0, expectedId, 0, 0, NL80211_CMD_SET_INTERFACE, 0);

        int name = if_nametoindex(uredaj.c_str());

        NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, name);
        NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, NL80211_IFTYPE_MONITOR);

        UgasiSucelje(uredaj);

        ret = nl_send_auto(socket, msg);
        // qDebug() << ret;

       nl_recvmsgs_default(socket);
       // qDebug() << ret;

        nl_socket_free(socket);
        nlmsg_free(msg);

        Postavi(uredaj);

        UpaliSucelje(uredaj);
        return true;

        nla_put_failure: // Mora se napraviti ovaj label jer NLA_PUT koristi taj goto :0
            nl_socket_free(socket);
            nlmsg_free(msg);

        return false;
    }

    bool PostaviUMonitorskiNacin(std::string uredaj){
        UgasiSucelje(uredaj);
        bool uspjeh = Postavi(uredaj);
        UpaliSucelje(uredaj);

        return uspjeh;
    }

    // Definirano ovdje identičnu funkciju onoj iz kernela
    // jer includanje linux/wireless.h istovremeno s linux/if.h u kojem se nalazi if_nametoindex
    // stvara probleme sa redefinicijama
    unsigned int if_nametoindex(const char *ifname)
    {
        int index;
        int ctl_sock;
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));
        strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = 0;
        index = 0;
        if ((ctl_sock = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
            if (ioctl(ctl_sock, SIOCGIFINDEX, &ifr) >= 0) {
                index = ifr.ifr_ifindex;
            }
            close(ctl_sock);
        }
        return index;
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
