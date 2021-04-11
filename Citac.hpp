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
#include <Cvor.hpp>
#include <Okvir.hpp>
#include <Paketi.h>

#include <QDebug>

namespace Citac {
    inline bool ugasi = false;

    int OtvoriSoket(std::string nazivSucelja);
    void PokreniCitanjePrometa(std::string nazivSucelja);
    void UgasiCitanjePrometa(std::string nazivSucelja);
    QList<Cvor> DohvatiSveCvorove();

    //std::mutex mtx;
    inline QList<Okvir> okviri;
    inline int RT_Len=26;
}

typedef std::vector<unsigned char> MACNiz;

enum rt_types {
    TSFT,
    FLAGS,
    RATE,
    CHANNEL,
    FHSS4,
    DBM_ANTSIGNAL,
    DBM_ANTNOISE,
    LOCK_QUALITY,
    TX_ATTENUATION,
    DB_TX_ATTENUATION,
    DBM_TX_POWER,
    ANTENNA,
    DB_ANTSIGNAL,
    DB_ANTNOISE,
    RX_FLAGS,
    TX_FLAGS,
    RTS_RETRIES,
    DATA_RETRIES,
    EXT = 31
};

struct Radiotap
{
    char Version;
    short Length;
    std::vector<int> Present;

    long TSFT;
    char Flags;
    unsigned int ChannelTx;
    unsigned int ChannelRx;
    unsigned short ChannelFreq;
    unsigned short	ChannelFlags;
    unsigned char		Rate;
    unsigned int	Fhss;
    char DbmSignal;
    short		DbmNoise;
    unsigned int LockQuality;
    unsigned int TxAttenuation;
    unsigned int DbTxAttenuation;
    short Power;
    unsigned short	Antenna;
    unsigned short	DbSignal;
    unsigned short		DbNoise;
};

template <typename T>
T ReadInt(std::vector<unsigned char> bytes)
{
    T ret = 0;
    for (int i = sizeof(T) - 1; i >= 0; --i)  // Little Endian!
    {

        unsigned char x = bytes[i];
        ret <<= 8;
        ret |= x;
    }
    return ret;
}

