#pragma once
#include "Font/BitmapFont.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)


struct BitmapFont;


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

    void Rebuild();
    
    // Увеличить масштаб
    void ZoomUp();
    
    // Уменьшить масштаб
    void ZoomDown();
    
    // Очистить символы, не входящие в основной набор
    void ClearBadSymbols();

    void ImportFont(wxTextFile &file, const wxString &nameFont);
    
    // Включить/выключить режим выбора символов
    void EnableModeSelectSymbols(bool enable);

    BitmapFont *GetFont();

private:

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    void Resize();

    void DrawCursor(wxPaintDC &dc);
    
    // Подсветить точку символа, на которую указывает мышь
    void HighlightPixel(wxPaintDC &dc);
    
    // Выделить/снять выделение с символа, который находчится под курсором
    void HighlightSymbol(wxPaintDC &dc);
    
    // Действующий размер холста
    wxSize CurrentSize();

    wxScrolledWindow *sw = nullptr;

    enum Mode
    {
        Edit,           // Основной режим, в котором можно редактировать символы
        SelectSymbols   // Режим выбора символов
    } mode = Edit;
};


extern Canvas *TheCanvas;
