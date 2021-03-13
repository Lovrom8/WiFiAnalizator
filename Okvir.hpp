#pragma once
#include <string>

struct MAC_Adr{
    std::string Adr1;
    std::string Adr2;
    std::string Adr3;
    std::string Adr4;
};

class Okvir{
public:
    MAC_Adr MAC;
    int JacinaSignala;
    std::string VrstaOkvira;
};
