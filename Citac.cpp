#include "Citac.hpp"

namespace Citac {

    int OtvoriSoket(std::string nazivSucelja){
        ifreq ifr;
        sockaddr_ll ll;

        int sock_raw = -1;
        sock_raw = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_802_2)); //htons(ETH_P_ALL));
        if(sock_raw == -1){
            qDebug() << "socket() greska: " << strerror(errno);
            return false;
        }

        strncpy(ifr.ifr_ifrn.ifrn_name, nazivSucelja.c_str(), sizeof(ifr.ifr_name));

        if(ioctl(sock_raw, SIOCGIFINDEX, &ifr) == -1){
            qDebug() << "Socket ioctl() greska: " << strerror(errno);
            return false;
        }

        memset(&ll, 0, sizeof(ll));
        ll.sll_ifindex = ifr.ifr_ifru.ifru_ivalue;
        ll.sll_protocol = htons(ETH_P_ALL);
        ll.sll_family = PF_PACKET;

        if(bind(sock_raw, (sockaddr*)&ll, sizeof(ll)) == -1){
            qDebug() << "Socket bind() greska: " << strerror(errno);
            qDebug() << errno;
            return false;
        }

        return sock_raw;
    }

    struct zaglavlje{
        short fc;
        short duration;
    };

    std::map<unsigned char, std::string> TipoviPaketa
    {
        {0b00000000, "Association Request (Management)"},
        {0b00010000, "Association Response (Management)"},
        {0b00100000, "Reassociation Request (Management)"},
        {0b00110000, "Reassociation Response (Management)"},
        {0b01000000, "Probe Request (Management)"},
        {0b01010000, "Probe Response (Management)"},
        {0b01100000, "Timing Advertisment (Management)"},
        {0b10000000, "Beacon (Management)"},
        {0b10110000, "Authentication (Management)"},
        {0b11000000, "Deauthentication (Management)"},
        {0b10010100, "Block ACK (Control)"},
        {0b11000100, "CTS (Control)"},
        {0b10110100, "RTS (Control)"},
        {0b11010100, "ACK (Control)"},
        {0b11001000, "QOS Null (Control)"}
    };

    std::string OdrediTip(unsigned char fcPrviBajt){
        std::string vrsta;

        if(TipoviPaketa.count(fcPrviBajt) > 0)
            vrsta = TipoviPaketa[fcPrviBajt].c_str();
        else
            vrsta = "Nepoznata vrsta paketa";

         qDebug() << vrsta.c_str();

         return vrsta;
    }

    enum PresentFlags{
        TSFT, // 8 0
        Flags, //1B 1
        Rate, //1B 2
        Channel, //2B 3
        FHSS, //1B 4
        dBmAntennaSignal, //1B 5
        dBmAntennaNoise, //1B 6
        LockQuality, //2B 7
        TXAttenuation, //2B 8
        dBTXAttenuation, //2B 9
        dBmTXPower, //1B 10
        Antenna, //1B 11
        dBAntennaSignal, //1B 12
        dBAntennaNoise, //1B 13
        RXFlags, //2B 14
        TXFlags, //2B 15
        ChannelPlus, //16
        MCSInformation, //17
        AMPDU, //18
        VHT, //19
        Timestamp, //20
        HEInfo, //21
        HEMU, //22
        LengthPSDU, //23
        LSIG, //24
        TLVs, //25
        RT_NS, //8B ??? //26
        V_NS, //3B //27
        Ext //0 - pomak za 4B
    };
                                                                            //16
    char RT_Velicine[32] = {8, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,3,0};

    struct radiotap_hdr {
        u_int8_t version;
        u_int8_t pad;
        u_int16_t length;
        u_int32_t present_flags;
    };

    bool ImaExt(int presentZastavice){ // Provjera postoji li više Present zastavica
        return presentZastavice & (1<<31);
    }

    void InterpretirajRT(char* paket){
        auto hdr = reinterpret_cast<radiotap_hdr*>(paket);

        unsigned int zastavice;
        int trenZastavica = 0;

        paket += 4; // Preskočimo do početka zastavica

        char *paketPom = paket;

        do{
            unsigned int* zastavice_ = reinterpret_cast<unsigned int*>(paketPom);
            zastavice = *zastavice_;
            paketPom += 4; // Pomakni do sljedeće ili zastavice ili početka polja s podatcima
        }while(ImaExt(zastavice));

        unsigned int* zastavice_ = reinterpret_cast<unsigned int*>(paket); //"Resetiraj" zastavice na prvu bitmasku
        zastavice = *zastavice_;
        qDebug() << "Prvi bajt podataka: " << QString::number(*paketPom, 16);

        paket = paketPom;
        QDebug deb = qDebug();
        /*for(int i = 0; i < 22;i++){
            unsigned char tren = paket[i];
                deb << QString::number(tren, 16);
        }*/

        while(zastavice > 0){
            if((zastavice & 1) == 1){
                qDebug() << QString::number(zastavice, 2);
                unsigned char tren = *paket;
                deb << QString::number(tren, 16) << " " << trenZastavica;
                //qDebug() << "Pomak: " << RT_Velicine[trenZastavica];
                paket += RT_Velicine[trenZastavica];
            }else{

            }

            trenZastavica++;
            //qDebug() << QString::number(paket[i], 16);
            zastavice >>= 1;
        }
    }

    int OdrediJacinuSignala(){

    }

    MAC_Adr OdrediMACAdrese(char* paketPtr){
        MAC_Adr adr;

        QString adresa = "";
        char *paket = paketPtr + 4; // Nakon 4 bajta počinje prva MAC adresa

        for(int i = 0; i < 6;i++)
            adresa += (QString::number(paket[i], 16)+":");

        adr.Adr1 = adresa.toStdString();

        return adr;
    }

    void ProcesirajPaket(int len, char* paket){
        //for(int i = 0; i < 26;i++)
        //            qDebug() << QString::number(paket[i], 16);

        //unsigned char signal = paket[20];
        //qDebug() << "Snaga signala" << QString::number(signal, 2);

        radiotap_hdr* rt_hdr = reinterpret_cast<radiotap_hdr*>(paket);
        qDebug() << "RT duljina: " << rt_hdr->length;

        InterpretirajRT(paket);

        //Prvih 26 bajtova je RADIOTAP HEADER (bar za ovaj adapter)
        paket = paket + 26;

        //zaglavlje* hdr = reinterpret_cast<zaglavlje*>(paket);
        /*for(int i = 0; i < len;i++)
            //qDebug() << QStringLiteral("%2").arg(paket[i]);
            //qDebug() << hex << paket[i];
            qDebug() << QString::number(paket[i], 16);*/

        unsigned char frameControlPrviBajt = paket[0]; // protocol version (b0 i b1), type (b2 i b3), subtype (b4,5,6 i 7)
        unsigned char frameControlDrugiBajt = paket[1];

        qDebug() << "Prvi FC bajt:" << QString::number(frameControlPrviBajt, 2);
        //qDebug() << QString::number(frameControlDrugiBajt, 2);

        //npr. za Probe response (50 00)
        // 0101    00    00
        // podtip  tip   verzija

        Okvir okvir;

        okvir.VrstaOkvira = OdrediTip(frameControlPrviBajt);
        okvir.JacinaSignala = OdrediJacinuSignala();
        okvir.MAC = OdrediMACAdrese(paket);

        okviri.append(okvir);
    }

    QList<Cvor> DohvatiSveCvorove(){
        QList<Cvor> cvorovi;

        //std::lock_guard<std::mutex> lock(mtx);

        //TODO: završi

        Cvor cvor;
        cvor.MAC_Adresa = "FF:FF:FF";
        cvorovi.append(cvor);
        cvor.MAC_Adresa = "AA:BB:CC";
        cvorovi.append(cvor);

        return cvorovi;
    }

    void DretvaSlusatelj(int rawSocket){
        bool ugasga = false;
        qDebug() << "Počinjem čitati";
        int msgLen = 0;

        char buffer[65536];
        while(!ugasga){
            memset(buffer, 0, 65536);
            if((msgLen = recvfrom(rawSocket, buffer, 65536, 0, NULL, NULL)) == -1){
                qDebug() << "Socket recv() greska: " << strerror(errno);
            }else{
                qDebug() << msgLen;
                buffer[msgLen-1] = '\0';
                ProcesirajPaket(msgLen, buffer);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void PokreniCitanjePrometa(std::string nazivSucelja) {
        int socket = -1;
        if((socket = OtvoriSoket(nazivSucelja)) == -1){
            qDebug() << "Neuspjesno otvarnje soketa";
            return;
        }
        qDebug() << "Uspješno otvoren sirovi soket na indeksu" << socket;
        qDebug() << "Otvaram dretvu sušatelj";
        std::thread thrd(DretvaSlusatelj, socket);
        thrd.detach();
    }
}
