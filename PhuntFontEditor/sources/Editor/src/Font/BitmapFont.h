#pragma once
#include "Dialogs/ImportSystemFontDialog.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include <vector>


class BitmapSymbol
{
    friend class SymbolImp;
    friend class FontImporter;
public:
    ~BitmapSymbol();
    int width = 0;
    int height = 0;

    void BuildFromFont(const wxFont &font, uint8 number, int w, int h, int offsetX, int offsetY);
    
    void Clear();

    void Draw(wxMemoryDC &dc, int x, int y);

    void Resize(int scale);
    
    // Сделать символ активным - будет в итоговом шрифте
    void Enable();
    
    // Сделать символ неактивным - не будет в итоговом шрифте
    void Disable();

    void ToggleState();

    bool IsEnabled() { return enabled; }

    bool IsEdited() { return edited; }

    static BitmapSymbol *Null();

    void TogglePixel(int col, int row);

    // Возвращает true, если даный пиксель закрашен
    bool GetPixel(int col, int row);

    void SetPixel(int col, int row);

    void ClearPixel(int col, int row);

private:
    
    // Здесь хранится исходное изображение в масштабе 1:1
    wxBitmap *bmpSymbol = nullptr;
    
    // Готовое к отрисовке изображение
    wxImage imgDrawable;
    
    // Если true, символ будет включён в итоговый шрифт
    bool enabled = true;

    // Если true, то символ отредактировал - в XML-файл нужно сохранять все пиксели символа - символ отредактирован и не соотвествует системному
    bool edited = false;

    static wxString UTFfrom1251(uint8 code);
};


struct BitmapFont
{
    ~BitmapFont();
    
    // Размер символа в точках
    wxSize size = { 0, 0 };
    
    // Столько писелей занимает одна точка шрифта в люому направлении
    int scale = 8;

    BitmapSymbol symbols[16][16];
    
    // Создаёт новый шрифт
    void CreateNew(const SettingsFont settings);

    void Resize();

    void Draw(wxPaintDC &dc);

    void DrawSymbol(BitmapSymbol *symbol);
    
    // Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

    BitmapSymbol *GetSymbol(int row, int col);

    BitmapSymbol *GetSymbol(uint8 num);
    
    // Возращает местоположение символа на холсте
    wxRect GetRectForSymbol(const BitmapSymbol *symbol) const;
    
    // Переключить состояине символа в позиции мыши с координатами mouseX, mouseY
    void ToggleStateSymbol(int mouseX, int mouseY);

    BitmapSymbol *GetSymbolUnderMouse(int x, int y);

    void GetSymbolXY(const BitmapSymbol *symbol, int *x, int *y);

    wxPoint GetCoordSymbol(const BitmapSymbol *symbol);

private:

    wxBitmap *bitmap = nullptr;
};
