#pragma once

#include "linux/if.h"
#include "linux/wireless.h"
#include "sys/socket.h"
#include "linux/if_packet.h"
#include "net/ethernet.h"
#include "sys/ioctl.h"
#include "arpa/inet.h"

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
#include "ProzorAnaliza.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QTimer>

typedef std::vector<unsigned char> MACNiz;
typedef std::vector<Okvir> ListaOkvira;

class Citac : public QObject {
    Q_OBJECT

public:
    void pokreni();
    int OtvoriSoket(QString nazivSucelja);
    void PokreniCitanjePrometa(QString nazivSucelja);
    void UgasiCitanjePrometa(QString nazivSucelja);
    void DretvaSlusatelj(int rawSocket);
    void DretvaCitatelj();
    QList<Cvor> DohvatiSveCvorove();

signals:
    void noviOkvir(Okvir okvir);
    void noviCvor(Cvor cvor);
    void osvjeziStatResp(std::vector<Okvir> _okviri);

public slots    :
     void osvjeziStatCitac();

 private:
    std::vector<QString> OdrediAdrese(unsigned char* bytes, Paket vrstaPaketa);
    QString DodajMAC(MACNiz MAC);
    std::vector<Okvir> sviOkviri;
    void Testiraj();
    bool ugasi = false;
    std::vector<MACNiz> MACAdr;
    QElapsedTimer timer;
    QTimer* statTimer;

    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;
    QMutex mutex;
    int numUsedBytes = 0;

};




