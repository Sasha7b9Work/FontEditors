#include "Editor.h"
#include "Dialogs/ImportFromXMLFileDialog.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/display.h>
#include <wx/fontdlg.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>
#pragma warning(pop)

#undef main


#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ExportFontToCFileDialog.h"
#include "Dialogs/ImportSystemFontDialog.h"

extern void init();

enum
{
    MENU_FILE_QUIT = wxID_EXIT
};


enum
{
    CONTEXT_MENU_DELETE = wxID_HIGHEST + 1,     // Удалить точку
    CONTEXT_MENU_PARAMETERS,                    // Параметры точки

    ALIGN_LEFT,                                 // Выровнять точку по левой
    ALIGN_RIGHT,                                // Выровнять точку по правой
    ALIGN_LEFT_TOP,                             // Выровнять точку по левой верхней
    ALIGN_LEFT_DOWN,                            // Выровнять точку по левой нижней
    ALIGN_RIGHT_TOP,                            // Выровнять точку по правой верхней
    ALIGN_RIGHT_DOWN,                           // Выровнять точку по правой нижней

    FILE_IMPORT_DESCRIPTION_FROM_XML,           // Импортировать шрифт из файла xml-описания
    FILE_IMPORT_SYSTEM_FONT,                    // Импортировать системный шрифт
    FILE_EXPORT_TO_C,                           // Экспортировать шрифт в си-файл
	UNDO,
	REDO,

    TOOL_SCALE_UP,                              // Увеличить масштаб
    TOOL_SCALE_DOWN,                            // Уменьшить масштаб
    TOOL_CLEAR_BAD_SYMBOLS,                     // Очистить сиволы из дополнительного набора
    TOOL_SELECT_SYMBOLS                         // Выбрать символы, которые будут включены в шрифт
};


wxIMPLEMENT_APP_NO_MAIN(Application); //-V2511


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    wxFont::AddPrivateFont(wxT("GOST.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_0.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_AU.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_BU.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_type_A.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_type_A_tilt.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_type_B.ttf"));
    wxFont::AddPrivateFont(wxT("GOST_typeB_tilt.ttf"));

    return wxEntry(argc, argv);
}


bool Application::OnInit() //-V2506
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


Frame::Frame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    CreateMenu();

    CreatePanels();

    CreateStatusBar(2);

    SetSizeAndPosition();

    Bind(wxEVT_MENU,     &Frame::OnQuit,                     this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU,     &Frame::OnImportDescriptionFromXML, this, FILE_IMPORT_DESCRIPTION_FROM_XML);
    Bind(wxEVT_MENU,     &Frame::OnImportSystemFont,         this, FILE_IMPORT_SYSTEM_FONT);
    Bind(wxEVT_MENU,     &Frame::OnExportFontToC,            this, FILE_EXPORT_TO_C);
    Bind(wxEVT_MENU,     &Frame::OnUndo,                     this, UNDO);
    Bind(wxEVT_MENU,     &Frame::OnRedo,                     this, REDO);
    Bind(wxEVT_SIZE,     &Frame::OnResize,                   this);
    Bind(wxEVT_PAINT,    &Frame::OnRepaint,                  this);
    Bind(wxEVT_KEY_DOWN, &Frame::OnKeyDown,                  this);
    Bind(wxEVT_MENU,     &Frame::OnZoomDown,                 this, TOOL_SCALE_DOWN);
    Bind(wxEVT_MENU,     &Frame::OnZoomUp,                   this, TOOL_SCALE_UP);
    Bind(wxEVT_MENU,     &Frame::OnClearBadSymbols,          this, TOOL_CLEAR_BAD_SYMBOLS);
    Bind(wxEVT_MENU,     &Frame::OnSelectSymbols,            this, TOOL_SELECT_SYMBOLS);

    Show(true);

    Maximize();
}


Frame::~Frame()
{
}

void Frame::CreatePanels()
{
    wxScrolledWindow *sw = new wxScrolledWindow(this); //-V2511

    TheCanvas = new Canvas(sw); //-V2511

    Centre();
}


void Frame::OnResize(wxSizeEvent &event)
{
    event.Skip();
}


void Frame::OnRepaint(wxPaintEvent &)
{
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}


void Frame::SetSizeAndPosition()
{
    wxSize size = { 1024, 768 };

	wxSize displaySize = wxGetDisplaySize();
	displaySize.y -= 40;

	if (displaySize.y < size.y)
	{
		size.y = displaySize.y;
	}

    SetSize(size);
    SetMinSize({ 640, 480 });

    wxRect rect = GetMaxDisplay();

    SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


wxRect Frame::GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Frame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu; //-V2511

    wxMenu *menuImport = new wxMenu; //-V2511
    menuImport->Append(FILE_IMPORT_DESCRIPTION_FROM_XML, wxT("Загрузить шрифт из xml-файла описания \tCtrl+O"));
    menuImport->Append(FILE_IMPORT_SYSTEM_FONT, wxT("Загрузить системный шрифт"));
    fileMenu->AppendSubMenu(menuImport, wxT("Импорт"));

    wxMenu *menuExport = new wxMenu; //-V2511
    menuExport->Append(FILE_EXPORT_TO_C, wxT("Экспортировать в си-файл"));
    fileMenu->AppendSubMenu(menuExport, wxT("Экспорт"));

    fileMenu->AppendSeparator();
    fileMenu->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenuBar *menuBar = new wxMenuBar(); //-V2511
    menuBar->Append(fileMenu, wxT("Файл"));

    SetMenuBar(menuBar);

    wxBitmap imgOpen(wxImage(wxT("icons/open.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgSave(wxImage(wxT("icons/save.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgChangeFont(wxImage(wxT("icons/font.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgUndo(wxImage(wxT("icons/undo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgRedo(wxImage(wxT("icons/redo.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleDown(wxImage(wxT("icons/minus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgScaleUp(wxImage(wxT("icons/plus.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgClear(wxImage(wxT("icons/clear.bmp"), wxBITMAP_TYPE_BMP));
    wxBitmap imgToggle(wxImage(wxT("icons/toggle.bmp"), wxBITMAP_TYPE_BMP));

    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(FILE_IMPORT_DESCRIPTION_FROM_XML, wxT(""), imgOpen, wxT("Загрузить шрифт из xml-файла описания"));
    toolBar->AddTool(FILE_EXPORT_TO_C, wxT(""), imgSave, wxT("Сохранить шрифт в си-файл"));

    toolBar->AddSeparator();
    toolBar->AddTool(UNDO, wxT("Отменить"), imgUndo, wxT("Отменить предыдущее действие"));
    toolBar->AddTool(REDO, wxT("Восстановить"), imgRedo, wxT("Восстановить следующее действие"));

    toolBar->AddSeparator();
    toolBar->AddTool(FILE_IMPORT_SYSTEM_FONT, wxT(""), imgChangeFont, wxT("Загрузить системный шрифт"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_SCALE_DOWN, wxT("Уменьшить масштаб"), imgScaleDown, wxT("Уменьшение масштаба"));
    toolBar->AddTool(TOOL_SCALE_UP, wxT("Увеличить масштаб"), imgScaleUp, wxT("Увеличение масштаба"));

    toolBar->AddSeparator();
    toolBar->AddTool(TOOL_CLEAR_BAD_SYMBOLS, wxT("Очистить дополнительные символы"), imgClear, wxT("Стереть дополнительные символы"));
    toolBar->AddTool(TOOL_SELECT_SYMBOLS, wxT("Выбрать символы для сохранения"), imgToggle, wxT(""), wxITEM_CHECK);

    toolBar->Realize();
}


void Frame::ShowContextMenu(const wxPoint &pos, bool underPoint)
{
    static wxMenu menuPoint;
    static wxMenu menuContext;
    static wxMenu *menuAlign = nullptr;

    if (menuAlign == nullptr)
    {
        menuAlign = new wxMenu(); //-V2511

        menuAlign->Append(ALIGN_LEFT, "Слева");
        menuAlign->Append(ALIGN_RIGHT, "Справа");
        menuAlign->Append(ALIGN_LEFT_TOP, "Слева вверху");
        menuAlign->Append(ALIGN_LEFT_DOWN, "Слева внизу");
        menuAlign->Append(ALIGN_RIGHT_TOP, "Справа вверху");
        menuAlign->Append(ALIGN_RIGHT_DOWN, "Справа внизу");

        menuPoint.Append(CONTEXT_MENU_DELETE, "Удалить");
        menuPoint.Append(CONTEXT_MENU_PARAMETERS, "Параметры");
        menuPoint.AppendSubMenu(menuAlign, "Выровнять");
    }

    PopupMenu(underPoint ? &menuPoint : &menuContext, pos.x, pos.y);
}


void Frame::OnImportDescriptionFromXML(wxCommandEvent &)
{
    ImportFromXMLFileDialog::Execute();
}


void Frame::OnExportFontToC(wxCommandEvent &)
{
    ExportFontToCFileDialog().ShowModal();
}


void Frame::OnKeyDown(wxKeyEvent &)
{
}


void Frame::OnUndo(wxCommandEvent &)
{

}


void Frame::OnRedo(wxCommandEvent &)
{

}


void Frame::OnZoomUp(wxCommandEvent &)
{
    TheCanvas->ZoomUp();
}


void Frame::OnZoomDown(wxCommandEvent &)
{
    TheCanvas->ZoomDown();
}


void Frame::OnImportSystemFont(wxCommandEvent &)
{
    ImportSystemFontDialog dlg(wxT("Импорт шрифта"));
    dlg.ShowModal();
}


void Frame::OnClearBadSymbols(wxCommandEvent &)
{
    TheCanvas->ClearBadSymbols();
}


void Frame::OnSelectSymbols(wxCommandEvent &event) //-V2009
{
    TheCanvas->EnableModeSelectSymbols(event.IsChecked());
}
