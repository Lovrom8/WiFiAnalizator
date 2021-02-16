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
        {0b00000000, "Association Request"},
        {0b00010000, "Association Response"},
        {0b00100000, "Reassociation Request"},
        {0b00110000, "Reassociation Response"},
        {0b01000000, "Probe Request"},
        {0b01010000, "Probe Response"},
        {0b01100000, "Timing Advertisment"},
        {0b10000000, "Beacon"},
        {0b10110000, "Authentication"},
        {0b11000000, "Deauthentication"},
        {0b10010100, "Block ACK"},
        {0b11000100, "CTS"},
        {0b10110100, "RTS"},
        {0b11010100, "ACK"},
        {0b11001000, "QOS Null"}
    };

    void OdrediTip(unsigned char fcPrviBajt){
        if(TipoviPaketa.count(fcPrviBajt) > 0)
            qDebug() << TipoviPaketa[fcPrviBajt].c_str();
        else
            qDebug() << "Nepoznata vrsta paketa";
    }

    struct radiotap_hdr {
        u_int8_t version;
        u_int8_t pad;
        u_int16_t length;
        u_int32_t present_flags;
    };

    enum PresentFlags{
        TSFT, // 8
        Flags, //1B
        Rate, //1B
        Channel, //2B
        FHSS, //1B
        dBmAntennaSignal, //1B
        dBmAntennaNoise, //1B
        LockQuality, //2B
        TXAttenuation, //2B
        dBTXAttenuation, //2B
        dBmTXPower, //1B
        Antenna, //1B
        dBAntennaSignal, //1B
        dBAntennaNoise, //1B
        RXFlags, //2B
        TXFlags, //2B
        ChannelPlus,
        MCSInformation,
        AMPDU,
        VHT,
        Timestamp,
        HEInfo,
        HEMU,
        LengthPSDU,
        LSIG,
        TLVs,
        RT_NS,
        V_NS,
        Ext //1b
    };

    short RT_Velicine[30] = {8, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 2};


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
            paketPom += 4; // Pomakni do sljedećeg ili zastavice ili početka polja s podatcima
        }while(ImaExt(zastavice));

        unsigned int* zastavice_ = reinterpret_cast<unsigned int*>(paketPom);
        zastavice = *zastavice_;
        qDebug() << "Prvi bajt podataka: " << QString::number(*paketPom, 16);

        paket = paketPom;
        while(zastavice > 0){
            if((zastavice & 1) == 1){
                unsigned char tren = *paket;
                qDebug() << QString::number(tren, 16);
                qDebug() << "Pomak: " << RT_Velicine[trenZastavica];
                paket += RT_Velicine[trenZastavica];
            }else{

            }

            trenZastavica++;
            //qDebug() << QString::number(paket[i], 16);
            zastavice >>= 1;
        }


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

        OdrediTip(frameControlPrviBajt);
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
                //ugasga = true;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            //qDebug() << buffer;
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
