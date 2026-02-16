#include "defines.h"
#include "files/Font.h"


#define BYTES_IN_ROW (width / 8)


//const uint8 *Symbol::GetRow(int row) const
//{
//    const uint8 *data = reinterpret_cast<const uint8 *>(&d);
//
//    return data + BytesInRow() * row;
//}
//
//
//uint8 Symbol::BytesInRow() const
//{
//    uint8 result = static_cast<uint8>(width / 8);
//
//    if (width % 8)
//    {
//        result++;
//    }
//
//    return result;
//}
//
//
//int Symbol::GetBit(int r, int c) const
//{
//    int byte = GetRow(r)[c / 8];
//
//    int bit = (1 << (c % 8));
//
//    return (byte & bit);
//}


//Symbol *Font::GetSymbol(uint8 code)
//{
//    uint16 *offsets = reinterpret_cast<uint16 *>(font);
//
//    if (offsets[code] == 0)
//    {
//        return nullptr;
//    }
//
//    return reinterpret_cast<Symbol *>(font + offsets[code]);
//}

