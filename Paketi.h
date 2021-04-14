#pragma once
#include <string>
#include <vector>

enum Adrese { // Ne zanima nas koja je vrsta adrese, već samo ima li je u određenoj vrsti okvira
    Ima,
    Nema,
    TBD
};

enum Vrste {
    Data,
    Managment,
    Control
};

struct AdrPolja {
    char Adr1, Adr2, Adr3, Adr4;

    AdrPolja() {
        Adr1 = ' ';
        Adr2 = ' ';
        Adr3 = ' ';
        Adr4 = ' ';
    }

    AdrPolja(char adr1, char adr2, char adr3, char adr4) {
        Adr1 = adr1;
        Adr2 = adr2;
        Adr3 = adr3;
        Adr4 = adr4;
    }
};

class Paket {
public:
    std::string Naziv;
    std::string Vrsta;
    AdrPolja AdresnaPolja;
    int Tip;

    Paket() { }

    Paket(std::string naziv, int tip) {
        Naziv = naziv;
        Tip = tip;
    }

    AdrPolja DohvatiAdrPolja(int ToDS, int FromDS) { // Samo za Data pakete
        if (ToDS == 1 && FromDS == 1)
            return AdrPolja(Ima, Ima, Ima, Ima);
        else
            return AdrPolja(Ima, Ima, Ima, Nema);
    }
};

class DataPaket : public Paket { // Prisustnost adresnih polja u Data paketima ovisi o ToDS i FromDS poljima
    //using Paket::Paket;
public:
    DataPaket(std::string naziv, int tip) : Paket(naziv, tip) {
        Vrsta = "Data";
    }
};

class MgmtPaket : public Paket { // Management paketi imaju fiksnu strukturu za dio s adresnim poljima
public:
    MgmtPaket(std::string naziv, int tip) : Paket(naziv, tip) {
        AdresnaPolja = AdrPolja(Ima, Ima, Ima, Nema);
        Vrsta = "Management";
    }
};

class ControlPaket : public Paket { // Svaka vrsta Control paketa ima svoju posebnu strukturu
public:
    ControlPaket(std::string naziv, int tip, AdrPolja adresnaPolja) : Paket(naziv, tip) {
        AdresnaPolja = adresnaPolja;
        Vrsta = "Control";
    }
};

class Paketi
{
private:
    std::vector<Paket> paketi;

public:
    Paketi() {

        /* MANAGMENT */
        paketi.push_back(MgmtPaket("Association Request", 0b00000000));
        paketi.push_back(MgmtPaket("Association Response", 0b00010000));
        paketi.push_back(MgmtPaket("Reassociation Request", 0b00100000));
        paketi.push_back(MgmtPaket("Reassociation Response", 0b00110000));
        paketi.push_back(MgmtPaket("Probe Request", 0b01000000));
        paketi.push_back(MgmtPaket("Probe Response", 0b01010000));
        paketi.push_back(MgmtPaket("Timing Advertisment", 0b01100000));
        paketi.push_back(MgmtPaket("Beacon", 0b10000000));
        paketi.push_back(MgmtPaket("ATIM", 0b10010000));
        paketi.push_back(MgmtPaket("Disassociation", 0b10100000));
        paketi.push_back(MgmtPaket("Authentication", 0b10110000));
        paketi.push_back(MgmtPaket("Deauthentication", 0b11000000));
        paketi.push_back(MgmtPaket("Action", 0b11010000));
        paketi.push_back(MgmtPaket("Action No Ack", 0b11100000));

        /* CONTROL */ // Izvađeno iz 802.11-2021.pdf
        paketi.push_back(ControlPaket("Trigger", 0b00100100, AdrPolja(Ima, Ima, Nema, Nema))); // https://www.semfionetworks.com/blog/the-80211ax-trigger-frame
        //paketi.push_back(ControlPaket("TACK", 0b00110100, AdrPolja()));
        paketi.push_back(ControlPaket("Beamforming Report Poll", 0b01000100, AdrPolja(Ima, Ima, Nema, Nema))); // https://www.hindawi.com/journals/tswj/2014/920937/
        paketi.push_back(ControlPaket("VHT/HE NDP Announcement", 0b01010100, AdrPolja(Ima, Ima, Nema, Nema)));
        //paketi.push_back(ControlPaket("Control Frame Extension", 0b01100100, AdrPolja()));
        paketi.push_back(ControlPaket("Block ACK", 0b10010100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("Control Wrapper", 0b01110100, AdrPolja(Ima, Nema, Nema, Nema)));
        paketi.push_back(ControlPaket("CTS", 0b11000100, AdrPolja(Ima, Nema, Nema, Nema)));
        paketi.push_back(ControlPaket("RTS", 0b10110100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("ACK", 0b11010100, AdrPolja(Ima, Nema, Nema, Nema)));
        paketi.push_back(ControlPaket("CF-End", 0b11100100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("CF-End + CF-ACK", 0b11110100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("PS-Poll", 0b10100100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("BlockAckReq", 0b01110100, AdrPolja(Ima, Ima, Nema, Nema)));
        paketi.push_back(ControlPaket("BlockAck", 0b10010100, AdrPolja(Ima, Ima, Nema, Nema)));

        /* DATA */
        paketi.push_back(DataPaket("Data", 0b00001000));
        paketi.push_back(DataPaket("Data + CF-ACK", 0b00011000));
        paketi.push_back(DataPaket("Data + CF-Poll", 0b00101000));
        paketi.push_back(DataPaket("Data + CF-ACK + CF-Poll", 0b00111000));
        paketi.push_back(DataPaket("Null (no data)", 0b01001000));
        paketi.push_back(DataPaket("CF-ACK (no data)", 0b01011000));
        paketi.push_back(DataPaket("CF-Poll (no data)", 0b01101000));
        paketi.push_back(DataPaket("CF-ACK + CF-Poll (no data)", 0b01111000));
        paketi.push_back(DataPaket("QoS Data", 0b10001000));
        paketi.push_back(DataPaket("QoS Data + CF-ACK", 0b10011000));
        paketi.push_back(DataPaket("QoS Data + CF-Poll", 0b10101000));
        paketi.push_back(DataPaket("QoS Data + CF-ACK + CF-Poll", 0b10111000));
        paketi.push_back(DataPaket("QoS Null (no data)", 0b11001000));
        paketi.push_back(DataPaket("Reserved", 0b11011000));
        paketi.push_back(DataPaket("QoS CF-Poll (no data)", 0b11101000));
        paketi.push_back(DataPaket("QoS CF-ACK + CF-Poll (no data)", 0b11111000));
    }


    std::vector<Paket> DohvatiPakete() {
        return paketi;
    }
};
