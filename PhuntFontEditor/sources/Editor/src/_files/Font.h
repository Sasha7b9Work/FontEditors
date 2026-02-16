#pragma once


//struct Symbol
//{
//    uint8 width;    // Ширина символа в битах
//    uint8 height;   // Высота символа в битах
//    uint8 firsRow;  // Номер первой непустой строки. Именно её первый байт записан в d
//    uint8 *d;       // Данные. Каждая строка имеет целое число байт. Лишние биты не используются. Младший бит идёт первым.
//
//    uint8 GetWidth() const { return width; }
//    uint8 GetHeight() const { return height; }
//    int GetBit(int row, int col) const;       // Возвращает 0, если точки нет, и произовльное значение в обратном случае
//    const uint8 *GetRow(int row) const;
//    uint8 BytesInRow() const;
//};
//
//
//struct HeaderFont
//{
//    uint16 offsets[256];            // Здесь хранятся смещения всех символов относительно &offsets[0]
//    Symbol *symbols;                // Первый символ. Со смещением 256
//};
//
//
//struct Font
//{
//    void SetFont(uint8 *f) { font = f; };
//    Symbol *GetSymbol(uint8 code);   // Возвращает указатель на символ с кодом code
//private:
//    uint8 *font;
//};
