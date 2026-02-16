#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Font/BitmapFont.h"
#include "Font/FontImporter.h"
#include <cstdlib>


Canvas *TheCanvas = nullptr;
static BitmapFont font;


static wxCoord mouseX;
static wxCoord mouseY;


Canvas::Canvas(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    sw = static_cast<wxScrolledWindow *>(parent);

    SetDoubleBuffered(true);

    Bind(wxEVT_PAINT,        &Canvas::OnPaint,          this);
    Bind(wxEVT_MOTION,       &Canvas::OnMouseMove,      this);
    Bind(wxEVT_LEFT_DOWN,    &Canvas::OnMouseLeftDown,  this);
    Bind(wxEVT_LEFT_DCLICK,  &Canvas::OnMouseLeftDown,  this);
    Bind(wxEVT_LEFT_UP,      &Canvas::OnMouseLeftUp,    this);
    Bind(wxEVT_RIGHT_DOWN,   &Canvas::OnMouseRightDown, this);
    Bind(wxEVT_RIGHT_UP,     &Canvas::OnMouseRightUp,   this);
    Bind(wxEVT_LEAVE_WINDOW, &Canvas::OnMouseLeave,     this);

    TuneScrollBar();

    Rebuild();
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK);

    dc.SetBrush(wxBrush(wxColor(0xF0, 0xF0, 0xF0)));

    dc.DrawRectangle(GetRect());

    font.Draw(dc);

    if (font.scale > 5)
    {
        dc.SetPen(wxPen(wxColor(0xa0, 0xa0, 0xa0)));

        for (int i = 0; i < GetSize().x; i += font.scale)
        {
            dc.DrawLine(i, 0, i, GetSize().y);
        }

        for (int i = 0; i < GetSize().y; i += font.scale)
        {
            dc.DrawLine(0, i, GetSize().x, i);
        }
    }

    dc.SetPen(*wxBLACK);
    
    for (int i = 0; i < GetSize().x; i += font.size.x * font.scale)
    {
        dc.DrawLine(i, 0, i, GetSize().y);
    }
    
    for (int i = 0; i < GetSize().y; i += font.size.y * font.scale)
    {
        dc.DrawLine(0, i, GetSize().x, i);
    }
    
    DrawCursor(dc);
}


void Canvas::DrawCursor(wxPaintDC &dc)
{
    if (mode == Mode::Edit)
    {
        HighlightPixel(dc);
    }
    else
    {
        HighlightSymbol(dc);
    }
}

static bool leftIsDown = false;
static bool rightIsDown = false;


static void DrawPixel(bool fill)
{
    BitmapSymbol *symbol = font.GetSymbolUnderMouse(mouseX, mouseY);
    wxRect rect = font.GetRectForSymbol(symbol);
    int dx = mouseX - rect.x;
    int dy = mouseY - rect.y;
    int row = dy / font.scale;
    int col = dx / font.scale;

    if (fill)   { symbol->SetPixel(col, row);   }
    else        { symbol->ClearPixel(col, row); }

    font.DrawSymbol(symbol);
}


void Canvas::OnMouseRightDown(wxMouseEvent &)
{
    rightIsDown = true;

    if (mode == Mode::Edit)
    {
        DrawPixel(false);
    }
}


void Canvas::OnMouseRightUp(wxMouseEvent &)
{
    rightIsDown = false;
}


void Canvas::OnMouseLeftDown(wxMouseEvent &)
{
    leftIsDown = true;

    if(mode == Mode::Edit)
    {
        DrawPixel(true);
    }
    else
    {
        font.ToggleStateSymbol(mouseX, mouseY);
    }

    Refresh();
}


void Canvas::OnMouseLeftUp(wxMouseEvent &)
{
    leftIsDown = false;
}


void Canvas::OnMouseMove(wxMouseEvent &event) //-V2009
{
    event.GetPosition(&mouseX, &mouseY);

    if (mode == Mode::Edit)
    {
        if (leftIsDown)
        {
            DrawPixel(true);
        }
        else if (rightIsDown) //-V2516
        {
            DrawPixel(false);
        }
    }

    Refresh();
}


void Canvas::HighlightPixel(wxPaintDC &dc)
{
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);

    int x = mouseX / font.scale * font.scale;
    int y = mouseY / font.scale * font.scale;

    dc.DrawRectangle(x, y, font.scale, font.scale);
}


void Canvas::HighlightSymbol(wxPaintDC &dc)
{
    BitmapSymbol *symbol = font.GetSymbolUnderMouse(mouseX, mouseY);

    dc.SetPen(wxPen(*wxBLUE, 5, wxPENSTYLE_SOLID));
    dc.SetBrush(wxBrush(*wxBLUE, wxBRUSHSTYLE_TRANSPARENT));

    wxRect rect = font.GetRectForSymbol(symbol);

    dc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);
    dc.DrawLine(rect.x, rect.y + rect.height, rect.x + rect.width, rect.y);
    dc.DrawRectangle(rect);
}


void Canvas::ZoomUp()
{
    if (font.scale < 32)
    {
        font.scale++;

        Resize();

        TuneScrollBar();

        Refresh();
    }
}


void Canvas::ZoomDown()
{
    if (font.scale > 2)
    {
        font.scale--;

        Resize();

        font.Resize();

        TuneScrollBar();

        Refresh();
    }
}


void Canvas::OnMouseLeave(wxMouseEvent &)
{
    mouseX = -100;
    mouseY = mouseX;
    Refresh();
}


wxSize Canvas::CurrentSize()
{
    return { 16 * font.size.x * font.scale + 1, 16 * font.size.y * font.scale + 1 };
}


void Canvas::TuneScrollBar()
{
    wxSize size = CurrentSize();

    sw->SetScrollbars(1, 1, size.x, size.y);

    SetPosition({ 0, 0 });
}


void Canvas::Rebuild()
{
    SettingsFont settings = ImportSystemFontDialog::GetSettingsFont();

    font.CreateNew(settings);

    SetSize(CurrentSize());

    Refresh();
}


void Canvas::Resize()
{
    SetSize(CurrentSize());
    font.Resize();
}


void Canvas::ClearBadSymbols()
{
    font.ClearBadSymbols();
    Refresh();
}


void Canvas::ImportFont(wxTextFile &file, const wxString &nameFont)
{
    FontImporter::Import(font, file, nameFont);
}


void Canvas::EnableModeSelectSymbols(bool enable)
{
    mode = enable ? Mode::SelectSymbols : Mode::Edit;
}


BitmapFont *Canvas::GetFont()
{
    return &font;
}
