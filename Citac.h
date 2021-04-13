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

#include <QDebug>

namespace Citac {
    inline bool ugasi = false;

    int OtvoriSoket(std::string nazivSucelja);
    void PokreniCitanjePrometa(std::string nazivSucelja);
    void UgasiCitanjePrometa(std::string nazivSucelja);
    QList<Cvor> DohvatiSveCvorove();

    //std::mutex mtx;
    inline int RT_Len=26;
}

typedef std::vector<unsigned char> MACNiz;






