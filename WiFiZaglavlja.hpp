#pragma once

struct FrameControl{
    /*
     2 bit - Protocol Version
     2 bit - Type
     4 bit - Sub type
     1 bit - To DS
     1 bit - From DS
     ...
    */
};

struct BeaconFrame{
    short FrameControl;
    short DurationID;
    char Adrress1[6];
    char Address2[6];
    char Address3[3];
    short SequenceControl[2];
    char Address4[6];
    //...
    int FCS;
};
