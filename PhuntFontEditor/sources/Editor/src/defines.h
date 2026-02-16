#pragma once


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;


#define BUTTON_SIZE wxSize(70, 30)


union BitSet16 //-V2514
{
    BitSet16(uint16 val) : halfword(val) {}
    uint8  byte[2];
    uint16 halfword;
};
