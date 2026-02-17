#pragma once
#pragma warning(push, 0)
#pragma warning(disable:4865 5039)
#include "wx/wx.h"
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);
    ~Frame();

    static Frame *self;

    void OnQuit(wxCommandEvent &);
    void OnResize(wxSizeEvent &);
    void OnRepaint(wxPaintEvent &);
    void OnZoomUp(wxCommandEvent &);
    void OnZoomDown(wxCommandEvent &);


private:
    void SetSizeAndPosition();

    wxRect GetMaxDisplay();

    void ShowContextMenu(const wxPoint &pos, bool underPoint);

    void CreateMenu();

    void CreatePanels();

    // Импортировать шрифт из xml-файла с описанием
    void OnImportDescriptionFromXML(wxCommandEvent &);

    // Импортировать системный шрифт
    void OnImportSystemFont(wxCommandEvent &);

    // Экспортировать шрифт в си-файл
    void OnExportFontToC(wxCommandEvent &);

    void OnUndo(wxCommandEvent &);
    void OnRedo(wxCommandEvent &);
    void OnKeyDown(wxKeyEvent &);

    // Очистить символы, не входящие в основной набор
    void OnClearBadSymbols(wxCommandEvent &);

    // Инструмент выбора сохраняемых символов
    void OnSelectSymbols(wxCommandEvent &);
};
