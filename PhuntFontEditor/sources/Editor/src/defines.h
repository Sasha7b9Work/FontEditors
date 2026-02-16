#pragma once


#pragma warning(push, 0)
#pragma warning(disable:4865 5039)
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>
#include <wx/display.h>
#include <wx/fontdlg.h>
#include <wx/mstream.h>
#pragma warning(pop)


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
