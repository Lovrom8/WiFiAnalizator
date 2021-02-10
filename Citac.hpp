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

#include <QDebug>

namespace Citac {
    inline bool ugasi = false;

    int OtvoriSoket(std::string nazivSucelja);
    void PokreniCitanjePrometa(std::string nazivSucelja);
    void UgasiCitanjePrometa(std::string nazivSucelja);
}
