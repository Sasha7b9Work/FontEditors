#include "defines.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Font/BitmapFont.h"
#include <thread>



BitmapSymbol::~BitmapSymbol()
{
    delete bmpSymbol; //-V2511
}


wxString BitmapSymbol::UTFfrom1251(uint8 code) //-V2506
{
    if (code >= 192)
    {
        return static_cast<wxChar>(0x0410 + code - 192);
    }

    if (code == 0x01)                                            // "ќмега"
    {
        return static_cast<wxChar>(0x03a9);
    }

    if (code == 184)
    {
        return static_cast<wxChar>(0x0451);
    }

    return wxString::Format("%c", code);
}


void BitmapSymbol::BuildFromFont(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY)
{
    width = w;
    height = h;

    delete bmpSymbol; //-V2511

    bmpSymbol = new wxBitmap(width, height); //-V2511

    wxMemoryDC memDC;

    memDC.SetFont(font);

    memDC.SelectObject(*bmpSymbol);

    memDC.SetPen(wxPen(*wxWHITE_PEN));
    memDC.SetBrush(wxBrush(*wxWHITE_BRUSH));

    memDC.Clear();

    memDC.SetPen(*wxBLACK_PEN);

    memDC.DrawText(BitmapSymbol::UTFfrom1251(number), { offsetX, offsetY });

    memDC.SelectObject(wxNullBitmap);
}


void BitmapSymbol::TogglePixel(int col, int row)
{
    if (GetPixel(col, row))
    {
        SetPixel(col, row);
    }
    else
    {
        ClearPixel(col, row);
    }
}


void BitmapSymbol::SetPixel(int col, int row)
{
    wxMemoryDC dc;

    dc.SelectObject(*bmpSymbol);
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.SetBrush(wxBrush(*wxBLACK_BRUSH));
    dc.DrawPoint(col, row);
    dc.SelectObject(wxNullBitmap);

    edited = true;
}


void BitmapSymbol::ClearPixel(int col, int row)
{
    wxMemoryDC dc;

    dc.SelectObject(*bmpSymbol);
    dc.SetPen(wxPen(*wxWHITE_PEN));
    dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    dc.DrawPoint(col, row);
    dc.SelectObject(wxNullBitmap);

    edited = true;

}


bool BitmapSymbol::GetPixel(int col, int row)
{
    wxMemoryDC dc;
    dc.SelectObject(*bmpSymbol);

    wxColour color;
    dc.GetPixel(col, row, &color);

    dc.SelectObject(wxNullBitmap);

    return color.Red() != 0;
}


void BitmapSymbol::Clear()
{
    delete bmpSymbol; //-V2511
    bmpSymbol = new wxBitmap(width, height); //-V2511

    wxMemoryDC memDC;
    memDC.SelectObject(*bmpSymbol);

    memDC.SetPen(*wxWHITE_PEN);
    memDC.SetBrush(*wxWHITE_BRUSH);

    memDC.DrawRectangle(0, 0, width, height);
}


void BitmapSymbol::Draw(wxMemoryDC &dc, int x, int y)
{
    dc.DrawBitmap(imgDrawable, x, y);
}


void BitmapSymbol::Resize(int scale)
{
    if (!enabled)
    {
        wxBitmap bitmap(*bmpSymbol);

        wxMemoryDC dc;
        dc.SelectObject(bitmap);
        dc.SetPen(*wxGREY_PEN);

        for (int x = 0; x < bitmap.GetWidth(); x++)
        {
            for (int y = 0; y < bitmap.GetHeight(); y++)
            {
                wxColour color;

                dc.GetPixel(x, y, &color);

                if (color.Red() == 0xFF)
                {
                    dc.DrawPoint(x, y);
                }
            }
        }

        imgDrawable = bitmap.ConvertToImage().Rescale(width * scale, height * scale);
    }
    else
    {
        imgDrawable = bmpSymbol->ConvertToImage().Rescale(width * scale, height * scale);
    }
}


static void FuncThread(BitmapSymbol symbols[16][16], int rowStart, int rowEnd, int scale)
{
    for(int row = rowStart; row < rowEnd; row++)
    {
        for(int col = 0; col < 16; col++)
        {
            symbols[row][col].Resize(scale);
        }
    }

}


void BitmapFont::Resize()
{
    std::thread thread0(FuncThread, symbols, 0, 4, scale);
    std::thread thread1(FuncThread, symbols, 4, 8, scale);
    std::thread thread2(FuncThread, symbols, 8, 12, scale);
    std::thread thread3(FuncThread, symbols, 12, 16, scale);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();

    delete bitmap; //-V2511

    bitmap = new wxBitmap(16 * size.x * scale, 16 * size.y * scale); //-V2511

    wxMemoryDC dc;
    dc.SelectObject(*bitmap);
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            int x0 = col * size.x * scale;
            int y0 = row * size.y * scale;

            symbols[row][col].Draw(dc, x0, y0);
        }
    }
}


void BitmapFont::DrawSymbol(BitmapSymbol *symbol)
{
    symbol->Resize(scale);
    
    wxMemoryDC dc;
    dc.SelectObject(*bitmap);

    wxPoint coord = GetCoordSymbol(symbol);
    symbol->Draw(dc, coord.x * size.x * scale, coord.y * size.y * scale);
}


wxPoint BitmapFont::GetCoordSymbol(const BitmapSymbol *symbol) //-V2506
{
    for(int row = 0; row < 16; row++)
    {
        for(int col = 0; col < 16; col++)
        {
            if(&symbols[row][col] == symbol)
            {
                return { col, row };
            }
        }
    }

    return { 0, 0 };
}


void BitmapFont::GetSymbolXY(const BitmapSymbol *symbol, int *x, int *y) //-V2506
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            if (symbol == &symbols[row][col])
            {
                *x = col * size.x * scale;
                *y = row * size.y * scale;
                return;
            }
        }
    }

    *x = 0;
    *y = 0;
}


void BitmapFont::CreateNew(const SettingsFont set) //-V801
{
    size.x = set.width;
    size.y = set.height;

    int i = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            symbols[row][col].BuildFromFont(set.font, static_cast<uint8>(i++), size.x, size.y, set.offsetX, set.offsetY);
        }
    }

    Resize();
}


void BitmapFont::Draw(wxPaintDC &dc)
{
    dc.DrawBitmap(*bitmap, 0, 0);
}


BitmapFont::~BitmapFont()
{
    delete bitmap; //-V2511
}


void BitmapFont::ClearBadSymbols()
{
    for (uint8 i = 0; i < 0x20; i++)
    {
        GetSymbol(i)->Disable();
        GetSymbol(i)->Clear();
    }

    for (uint8 i = 0x80; i < 0xc0; i++)
    {
        GetSymbol(i)->Disable();
        GetSymbol(i)->Clear();
    }

    Resize();
}


void BitmapSymbol::Disable()
{
    enabled = false;
}


void BitmapSymbol::Enable()
{
    enabled = true;
}


void BitmapSymbol::ToggleState()
{
    enabled = !enabled;
}


BitmapSymbol *BitmapFont::GetSymbol(int row, int col)
{
    return &symbols[row][col];
}


BitmapSymbol *BitmapFont::GetSymbol(uint8 num)
{
    BitmapSymbol *symbol = &symbols[0][0];

    return &symbol[num];
}


wxRect BitmapFont::GetRectForSymbol(const BitmapSymbol *symbol) const //-V2506
{
    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            if (&symbols[row][col] == symbol)
            {
                wxRect rect;

                rect.x = col * size.x * scale;
                rect.y = row * size.y * scale;
                rect.width = size.x * scale;
                rect.height = size.y * scale;

                return rect;
            }
        }
    }

    return wxRect();
}


BitmapSymbol *BitmapSymbol::Null()
{
    static BitmapSymbol symbol;
    return &symbol;
}


void BitmapFont::ToggleStateSymbol(int mouseX, int mouseY)
{
    BitmapSymbol *symbol = GetSymbolUnderMouse(mouseX, mouseY);

    symbol->ToggleState();
    symbol->Resize(scale);

    int x = 0, y = 0;

    GetSymbolXY(symbol, &x, &y);

    wxMemoryDC dc;

    dc.SelectObject(*bitmap);

    symbol->Draw(dc, x, y);
}


BitmapSymbol *BitmapFont::GetSymbolUnderMouse(int x, int y) //-V2506
{
    if (x < 0 || y < 0)
    {
        return BitmapSymbol::Null();
    }

    int col = x / size.x / scale;
    int row = y / size.y / scale;

    return GetSymbol(row, col);
}
