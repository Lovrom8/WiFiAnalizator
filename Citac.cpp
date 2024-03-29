#include "Citac.h"

int Citac::OtvoriSoket(QString nazivSucelja){
    ifreq ifr;
    sockaddr_ll ll;

    int sock_raw = -1;
    sock_raw = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_802_2)); //htons(ETH_P_ALL));
    if(sock_raw == -1){
        qDebug() << "socket() greska: " << strerror(errno);
        return false;
    }

    strncpy(ifr.ifr_ifrn.ifrn_name, nazivSucelja.toStdString().c_str(), sizeof(ifr.ifr_name));

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

void Citac::Testiraj(){
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

    using namespace Procesiranje;
    Paket pak0 = *(Procesiranje::OdrediVrstu(bytes));
    Paket pak1 = *(Procesiranje::OdrediVrstu(bytes1));
    Paket pak2 = *(Procesiranje::OdrediVrstu(bytes2));
    Paket pak3 = *(Procesiranje::OdrediVrstu(bytes3));
    Paket pak4 = *(Procesiranje::OdrediVrstu(bytesData));

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

bool JeBroadcastMAC(std::vector<unsigned char> MAC) {
    return std::equal(MAC.begin() + 1, MAC.end(), MAC.begin()) && MAC[0] == 0xFF;
}

int emitted = 0;
QString Citac::DodajMAC(MACNiz MAC) {
    QString adresa = "";

    if(MAC.size() != 6) {
        //TODO: provjeri u kojim je ovo slučajevima
        return adresa;
    }

    for(int i = 0; i < 6;i++){
        adresa += QString::number(MAC[i], 16);
        adresa += ":";
    }

    adresa.chop(1);

    if (std::find(MACAdr.begin(), MACAdr.end(), MAC) == std::end(MACAdr) && !JeBroadcastMAC(MAC)) {
        MACAdr.push_back(MAC);

        Cvor cvor;
        cvor.MAC_Adresa = adresa;

        emit noviCvor(cvor);
        emitted++;

        //qDebug() << "Emmited. ";
        //qDebug() << emitted << " " << MACAdr.size();
    }

    return adresa;
}

void Citac::osvjeziStatCitac() {
    emit osvjeziStatResp(sviOkviri);
}

std::vector<QString> Citac::OdrediAdrese(unsigned char* bytes, Paket vrstaPaketa) {
    int rtLen = Procesiranje::OdrediDuljinuRT(bytes);
    int trenByte = rtLen + 4;
    unsigned char* adreseBytes = bytes;

    AdrPolja polja = vrstaPaketa.AdresnaPolja;

    std::vector<QString> macAdrese;

    if (vrstaPaketa.Vrsta == "Data") {
        unsigned char FCZastavice = bytes[rtLen + 1];

        int ToDS = (FCZastavice & 1) == 1;
        FCZastavice >>= 1;
        int FromDS = (FCZastavice & 1) == 1;

        polja = vrstaPaketa.DohvatiAdrPolja(ToDS, FromDS);
    }

    if (polja.Adr1 == Ima) {
        macAdrese.push_back(DodajMAC(Procesiranje::Split(adreseBytes, trenByte, 6)));
        trenByte += 6;
    }

    if (polja.Adr2 == Ima) {
        macAdrese.push_back(DodajMAC(Procesiranje::Split(adreseBytes, trenByte, 6)));
        trenByte += 6;
    }

    if (polja.Adr3 == Ima) {
        macAdrese.push_back(DodajMAC(Procesiranje::Split(adreseBytes, trenByte, 6)));
        trenByte += 6;
    }

    if (polja.Adr4 == Ima)
        macAdrese.push_back(DodajMAC(Procesiranje::Split(adreseBytes, trenByte, 6)));

    return macAdrese;
}

void Citac::DretvaSlusatelj(int rawSocket){
    bool ugasga = false;
    qDebug() << "Počinjem čitati";
    int msgLen = 0;

    unsigned char buffer[65536];
    while(!ugasga){
        memset(buffer, 0, 65536);
        if((msgLen = recvfrom(rawSocket, buffer, 65536, 0, NULL, NULL)) == -1){
            qDebug() << "Socket recv() greska: " << strerror(errno);
        }else{
            //qDebug() << msgLen;
            buffer[msgLen] = '\0';

            auto okvir = Procesiranje::ProcesirajPaket(msgLen, buffer);
            okvir.Vrijeme = (double)timer.elapsed() / 1000;

            if(okvir.paket) //JIC again
                okvir.macAdrese = OdrediAdrese(buffer, *(okvir.paket));

            emit noviOkvir(okvir);
            sviOkviri.push_back(okvir);
        }
    }
}

void Citac::PokreniCitanjePrometa(QString nazivSucelja) {
    //Testiraj();
    // return;
    timer.start();

    qRegisterMetaType<Okvir>("Okvir"); //VAŽNO!
    qRegisterMetaType<Cvor>("Cvor");
    qRegisterMetaType<std::vector<Cvor>>("Cvorovi");

    int socket = -1;
    if((socket = OtvoriSoket(nazivSucelja)) == -1){
        qDebug() << "Neuspjesno otvarnje soketa";
        return;
    }
    qDebug() << "Uspješno otvoren sirovi soket na indeksu" << socket;
    qDebug() << "Otvaram dretvu sušatelj";

    QThread *thread = QThread::create(&Citac::DretvaSlusatelj, this, socket);
    thread->start();
}
