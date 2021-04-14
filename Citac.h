#pragma once

#include "linux/if.h"
#include "linux/wireless.h"
#include "sys/socket.h"
#include "linux/if_packet.h"
#include "net/ethernet.h"
#include "sys/ioctl.h"
#include "arpa/inet.h"
#include "ProzorAnaliza.h"

#include "string.h"
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>

#include <Cvor.h>
#include <Okvir.h>
#include <Paketi.h>
#include "Procesiranje.h"

#include <QDebug>
#include <QElapsedTimer>

class Citac : public QObject {
    Q_OBJECT

public:
    void pokreni();
    int OtvoriSoket(std::string nazivSucelja);
    void PokreniCitanjePrometa(std::string nazivSucelja);
    void UgasiCitanjePrometa(std::string nazivSucelja);
    void DretvaSlusatelj(int rawSocket);
    QList<Cvor> DohvatiSveCvorove();

signals:
    void noviOkvir(Okvir okvir);
    void noviCvor(Cvor cvor);

 private:
    void OdrediAdrese(unsigned char* bytes, Paket vrstaPaketa);
    void DodajMAC(std::vector<unsigned char> MAC);
    void Testiraj();
    bool ugasi = false;
    QElapsedTimer timer;
};

typedef std::vector<unsigned char> MACNiz;



