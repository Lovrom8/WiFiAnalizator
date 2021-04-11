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

    bool ImaExt(int presentZastavice){ // Provjera postoji li više Present zastavica
        return presentZastavice & (1<<31);
    }

    int LEN = 26;
    std::vector<unsigned char> Split(unsigned char* bytes, int from, int len) {
        std::vector<unsigned char> bytesRet;

        int arrLen = LEN;
        int to = from + len;
        for (int i = 0; i < arrLen; i++)
            if (i >= from && i < to)
                bytesRet.push_back(bytes[i]);

        return bytesRet;
    }

    int BrojPresenta(unsigned char* bajtovi) {
        int trenBit = 0, br = 1;
        bool imaJos = true;

        auto hdrPresent = Split(bajtovi, 4, 4);
        unsigned int zastavice = ReadInt<unsigned int>(hdrPresent);
        unsigned char* pocetni = bajtovi + 4;

        do {
            while (zastavice > 0) {
                if (trenBit == EXT) {
                    if ((zastavice & 1) == 1) { // Ako je EXT postavljen na 1, idemo na sljedeći
                        br++;

                        hdrPresent = Split(pocetni, 4 * (br - 1), 4);
                        zastavice = ReadInt<unsigned int>(hdrPresent);
                        trenBit = 0;
                    }
                    else
                        imaJos = false;
                }
                else {
                    if (trenBit == 15 || trenBit == 23) { // 16.-ti i 24.-ti bit su neiskorišteni
                        zastavice >>= 1;
                        trenBit++;
                    }

                    zastavice >>= 1;
                    trenBit++;

                    if (zastavice == 0 && trenBit != EXT) { // Ako je bitmapa kraća od 32 bita, sigurno poslje nje nam novih
                        imaJos = false;
                    }
                }
            }
        } while (imaJos);

        return br;
    }


    Radiotap* ProcesirajRT(unsigned char* bajtovi) {
        Radiotap* rt = new Radiotap;

        rt->Version = bajtovi[0];

        auto hdrLen = Split(bajtovi, 2, 2);
        rt->Length = ReadInt<unsigned short>(hdrLen);

        auto hdrPresent = Split(bajtovi, 4, 4);
        unsigned int zastavice = ReadInt<unsigned int>(hdrPresent);
        rt->Present.push_back(zastavice);

        int trenBit = 0;

        int brPresentBitmaski = BrojPresenta(bajtovi);

        bajtovi += (4 + brPresentBitmaski * 4); // Preskocimo sve present bitmaske (svaka od njih ima 4 bajta)
        int trenByte = 0;

        while (zastavice > 0) {
            if ((zastavice & 1) == 1) {

                //unsigned char trenZastavica = *bytes;
                //deb << QString::number(tren, 16) << " " << trenZastavica;

                switch (trenBit) { // Pomičemo se ovisno o prisutnosti bit-ova postavljenih na 1
                    case (TSFT): {
                        trenByte += 8;
                        rt->TSFT = ReadInt<long>(Split(bajtovi, trenByte, 8));
                        trenByte += 8;
                        break;
                    }

                    case (FLAGS): {
                        rt->Flags = ReadInt<unsigned char>(Split(bajtovi, trenByte, 1));
                        trenByte += 1;
                        break;
                    }

                    case (RATE): {
                        auto crap = Split(bajtovi, trenByte, 1);

                        rt->Rate = ReadInt<unsigned char>(crap) / 2; // Vrijednost je u 500Kpbs pa dijeliti sa dva za Mb/s
                        trenByte += 1;
                        break;
                    }

                    case (CHANNEL): {
                        rt->ChannelFreq = ReadInt<unsigned short>(Split(bajtovi, trenByte, 2));
                        trenByte += 2;

                        rt->ChannelFlags = ReadInt<unsigned short>(Split(bajtovi, trenByte, 2));
                        trenByte += 2;
                        break;
                    }

                    case (DBM_ANTSIGNAL): { // Posljednji koji nas zanima je jačina signala u DBm
                        unsigned char signalByte = ReadInt<unsigned char>(Split(bajtovi, trenByte, 1));
                        rt->DbmSignal = (~signalByte + 1) * -1;
                        trenByte += 1;
                        break;
                    }
                }

                //std::cout << tren << " : " << HEX(trenZastavica) << " " << std::dec;
                //std::cout << tren << " : " << 1 << " " << std::dec;
            }
            else {
                //std::cout << 0 << " ";

                if (trenBit == EXT) {
                    zastavice = 0;
                }
            }

            //qDebug() << QString::number(paket[i], 16);

            if (trenBit == 15 || trenBit == 23) {
                zastavice >>= 1;
                trenBit++;
                //std::cout << 0 << " ";
            }

            zastavice >>= 1;
            trenBit++;
        }

        return rt;
    }

    int OdrediJacinuSignala(unsigned char* paket){
        std::unique_ptr<Radiotap> rtHdr(ProcesirajRT(paket));

        return rtHdr->DbmSignal;
    }


    int OdrediDuljinuRT(unsigned char* bytes) {
        auto hdrLen = Split(bytes, 2, 2);
        return ReadInt<unsigned short>(hdrLen);
    }

    std::vector<std::vector<unsigned char>> MACAdr;
    bool JeBroadcastMAC(std::vector<unsigned char> MAC) {
        return std::equal(MAC.begin() + 1, MAC.end(), MAC.begin()) && MAC[0] == 0xFF;
    }

    void DodajMAC(std::vector<unsigned char> MAC) {
        if (!std::count(MACAdr.begin(), MACAdr.end(), MAC) && !JeBroadcastMAC(MAC))
            MACAdr.push_back(MAC);
    }

    void OdrediAdrese(unsigned char* bytes, Paket vrstaPaketa) {
        int rtLen = OdrediDuljinuRT(bytes);
        int trenByte = rtLen + 4;
        unsigned char* adreseBytes = bytes;

        AdrPolja polja = vrstaPaketa.AdresnaPolja;

        if (vrstaPaketa.Vrsta == "Data") {
            unsigned char FCZastavice = bytes[rtLen + 1];

            int ToDS = (FCZastavice & 1) == 1;
            FCZastavice >>= 1;
            int FromDS = (FCZastavice & 1) == 1;

            polja = vrstaPaketa.DohvatiAdrPolja(ToDS, FromDS);
        }

        if (polja.Adr1 == Ima) {
            DodajMAC(Split(adreseBytes, trenByte, 6));
            trenByte += 6;
        }

        if (polja.Adr2 == Ima) {
            DodajMAC(Split(adreseBytes, trenByte, 6));
            trenByte += 6;
        }

        if (polja.Adr3 == Ima) {
            DodajMAC(Split(adreseBytes, trenByte, 6));
            trenByte += 6;
        }

        if (polja.Adr4 == Ima)
            DodajMAC(Split(adreseBytes, trenByte, 6));
    }

    Paket OdrediVrstu(unsigned char* bytes) {
        Paketi paketi;
        auto vrstePaketa = paketi.DohvatiPakete();

        unsigned char* pom = bytes + OdrediDuljinuRT(bytes);
        int x = pom[0];
        for (Paket paket : vrstePaketa) {
            if (paket.Tip == x) {
                //std::cout << paket.Naziv << std::endl;
                qDebug() << paket.Naziv.c_str();
                return paket;
            }
        }
    }


    void ProcesirajPaket(int len, unsigned char* paket){
        unsigned char *paket_ = paket;
        paket = paket + RT_Len;   //Prvih 26 bajtova je RADIOTAP HEADER (bar za ovaj adapter)

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

        okvir.VrstaOkvira = OdrediVrstu(paket_).Vrsta;
        okvir.JacinaSignala = OdrediJacinuSignala(paket_);
        //okvir.MAC = OdrediMACAdrese(paket);

        okviri.append(okvir);
    }

    QList<Cvor> DohvatiSveCvorove(){
        QList<Cvor> cvorovi;

        //std::lock_guard<std::mutex> lock(mtx);

        //TODO: završi

        for(MACNiz mac : MACAdr){
            Cvor cvor;
            cvor.MAC_Adresa = "";
            for(int i = 0; i < 6;i++){
                cvor.MAC_Adresa += QString::number(mac[i], 16);
                cvor.MAC_Adresa += ":";
            }

            cvorovi.append(cvor);
        }

        return cvorovi;
    }

    void Testiraj(){
        unsigned char bytes[100] = { // RTS
                 0x00 ,0x00 ,0x1a ,0x00 ,0xae ,0x40 ,0x00 ,0xa0 ,0x20 ,0x08 ,0x00 ,0x00 ,0x10 ,0x02 ,0x6c ,0x09,
                 0xa0 ,0x00 ,0xc0 ,0x00 ,0x3f ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xb4 ,0x00 ,0xb4 ,0x01 ,0x88 ,0x5b,
                 0xdd ,0x70 ,0x84 ,0x54 ,0x70 ,0xc9 ,0x4e ,0x5a ,0xcb ,0xdb ,0x01 ,0x96 ,0xcb ,0xd2
            };

            unsigned char bytes1[100] = { // Block ACK
                0x00 ,0x00 ,0x1a ,0x00 ,0xae ,0x40 ,0x00 ,0xa0 ,0x20 ,0x08 ,0x00 ,0x00 ,0x10 ,0x30 ,0x6c ,0x09,
                0xc0 ,0x00 ,0xb5 ,0x00 ,0x27 ,0x00 ,0x00 ,0x00 ,0xb6 ,0x00 ,0x94 ,0x00 ,0x00 ,0x00 ,0x7c ,0xa1, // 7C A1 77 B4 F1 46
                0x77 ,0xb4 ,0xf1 ,0x46 ,0x88 ,0x5b ,0xdd ,0xea ,0x50 ,0x14 ,0x05 ,0x00 ,0xb0 ,0x65 ,0x01 ,0x00, // 88 5B DD EA 50 14
                0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xa6 ,0x4d ,0x36 ,0x85
            };

            unsigned char bytes2[100] = { // CTS
                0x00 ,0x00 ,0x1a ,0x00 ,0xae ,0x40 ,0x00 ,0xa0 ,0x20 ,0x08 ,0x00 ,0x00 ,0x90 ,0x16 ,0x6c ,0x09,
                0xa0 ,0x00 ,0xcc ,0x00 ,0x64 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xc4 ,0x00 ,0x4a ,0x01 ,0x34 ,0x2e,
                0xb7 ,0x91 ,0x70 ,0xdf ,0xb5 ,0xd5 ,0xc9 ,0x28
            };

            unsigned char bytes3[200] = { // Probe Request
                0x00 ,0x00 ,0x1a ,0x00 ,0xae ,0x40 ,0x00 ,0xa0 ,0x20 ,0x08 ,0x00 ,0x00 ,0x10 ,0x02 ,0x6c ,0x09,
                0xa0 ,0x00 ,0xc0 ,0x00 ,0x3f ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x00 ,0x00 ,0x00 ,0xff ,0xff,
                0xff ,0xff ,0xff ,0xff ,0x28 ,0x39 ,0x26 ,0xa3 ,0x6a ,0xb1 ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff,
                0xb0 ,0xab ,0x00 ,0x00 ,0x01 ,0x08 ,0x82 ,0x84 ,0x8b ,0x96 ,0x0c ,0x12 ,0x18 ,0x24 ,0x32 ,0x04,
                0x30 ,0x48 ,0x60 ,0x6c ,0x2d ,0x1a ,0x2c ,0x08 ,0x02 ,0xff ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
                0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
                0x46 ,0x05 ,0x32 ,0x00 ,0x01 ,0x00 ,0x00 ,0x7f ,0x09 ,0x00 ,0x00 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00,
                0x00 ,0x00 ,0xf9 ,0x78 ,0x7f ,0x2f
            };

            unsigned char bytesData[400] = { // Data
                0x00 ,0x00 ,0x1a ,0x00 ,0xae ,0x40 ,0x00 ,0xa0 ,0x20 ,0x08 ,0x00 ,0x00 ,0x10 ,0x16 ,0x6c ,0x09,
                0xa0 ,0x00 ,0xca ,0x00 ,0x64 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x08 ,0x62 ,0x00 ,0x00 ,0xff ,0xff,
                0xff ,0xff ,0xff ,0xff ,0x88 ,0x5b ,0xdd ,0xea ,0x50 ,0x14 ,0x10 ,0x62 ,0xe5 ,0x90 ,0x62 ,0x90,
                0x00 ,0xbf ,0x7e ,0xb6 ,0x00 ,0x60 ,0x09 ,0x02 ,0x00 ,0x00 ,0x32 ,0x78 ,0x72 ,0x05 ,0x18 ,0x6d,
                0x86 ,0x77 ,0x9a ,0x1a ,0xc8 ,0xcb ,0x74 ,0x60 ,0x73 ,0x8b ,0x97 ,0x75 ,0x9e ,0xf2 ,0xa0 ,0x96,
                0x74 ,0xc8 ,0xb7 ,0xcc ,0xef ,0x78 ,0xdb ,0x4e ,0x5e ,0x0d ,0x6c ,0x86 ,0x63 ,0x65 ,0xf7 ,0x22,
                0x55 ,0x49 ,0x14 ,0x36 ,0x97 ,0x57 ,0x77 ,0xa9 ,0xca ,0x9c ,0x3b ,0x0b ,0x0c ,0xc0 ,0x0a ,0x69,
                0xb3 ,0x6f ,0x0b ,0xa6 ,0xfa ,0x36 ,0x24 ,0xf2 ,0x0d ,0x1b ,0x8b ,0x16 ,0x41 ,0xa6
            };

            Paket pak0 = OdrediVrstu(bytes);
            Paket pak1 = OdrediVrstu(bytes1);
            Paket pak2 = OdrediVrstu(bytes2);
            Paket pak3 = OdrediVrstu(bytes3);
            Paket pak4 = OdrediVrstu(bytesData);

            LEN = 46;
            OdrediAdrese(bytes, pak0);
            LEN = 58;
            OdrediAdrese(bytes1, pak1);
            LEN = 40;
            OdrediAdrese(bytes2, pak2);
            LEN = 153;
            OdrediAdrese(bytes3, pak3);
            LEN = 126;
            OdrediAdrese(bytesData, pak4);


    }

    void DretvaSlusatelj(int rawSocket){
        bool ugasga = false;
        qDebug() << "Počinjem čitati";
        int msgLen = 0;

        unsigned char buffer[65536];
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
        Testiraj();
        return;

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
