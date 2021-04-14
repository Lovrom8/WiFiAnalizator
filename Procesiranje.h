#pragma once
#include <vector>
#include <unistd.h>
#include <memory>
#include <algorithm>
#include <QDebug>

#include "Paketi.h"
#include "Okvir.h"

enum RTTypes {
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

namespace Procesiranje
{
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

         int BrojPresenta(unsigned char* bajtovi);
         Radiotap *ProcesirajRT(unsigned char* bajtovi);

         int OdrediJacinuSignala(unsigned char* paket);
         int OdrediDuljinuRT(unsigned char* bytes);

         bool JeBroadcastMAC(std::vector<unsigned char> MAC);
         void DodajMAC(std::vector<unsigned char> MAC);

         Paket OdrediVrstu(unsigned char* bytes);
         Okvir ProcesirajPaket(int len, unsigned char* paket);

         std::vector<unsigned char> Split(unsigned char* bytes, int from, int len);

         inline int RT_LEN = 26;
         inline int LEN = 26;
         inline QList<Okvir> okviri;
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


