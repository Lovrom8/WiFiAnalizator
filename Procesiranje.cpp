#include "Procesiranje.h"

namespace Procesiranje {
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

    bool ImaExt(int presentZastavice){ // Provjera postoji li više Present zastavica
        return presentZastavice & (1<<31);
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
        int tip = pom[0];
       /* for (Paket paket : vrstePaketa) {
            if (paket.Tip == tip) {
                //std::cout << paket.Naziv << std::endl;
                qDebug() << paket.Naziv.c_str();
                return paket;
            }
        }*/

        auto paket = *(std::find_if(std::begin(vrstePaketa), std::end(vrstePaketa), [&](Paket &paket) { return paket.Tip == tip; }));
        qDebug() << paket.Naziv.c_str();
        return paket;
    }


    Okvir ProcesirajPaket(int len, unsigned char* paket){
        unsigned char *paketPocetak = paket;
        paket = paket + OdrediDuljinuRT(paket);   //Prvih 26 bajtova je RADIOTAP HEADER (bar za ovaj adapter)

        LEN = len;

        unsigned char frameControlPrviBajt = paket[0]; // protocol version (b0 i b1), type (b2 i b3), subtype (b4,5,6 i 7)
        unsigned char frameControlDrugiBajt = paket[1];

        //qDebug() << "Prvi FC bajt:" << QString::number(frameControlPrviBajt, 2);
        //qDebug() << QString::number(frameControlDrugiBajt, 2);

        //npr. za Probe response (50 00)
        // 0101    00    00
        // podtip  tip   verzija

        Okvir okvir;

        okvir.VrstaOkvira = OdrediVrstu(paketPocetak).Naziv;
        okvir.JacinaSignala = OdrediJacinuSignala(paketPocetak);
        //okvir.MAC = OdrediMACAdrese(paketPocetak);

        return okvir;
       // okviri.append(okvir);
    }
}

