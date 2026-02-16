#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Symbol;
class wxXmlNode;


class ExportFontToCFileDialog : public wxDialog
{
public:
    ExportFontToCFileDialog();

private:
    void OnButtonExport(wxCommandEvent &);
    void OnButtonCancel(wxCommandEvent &);

    // Записать в файл информацию о шрифте
    void WriteFileXML(const wxString &nameFileFont);

    // Записывает информацию о сиволе в подузел узла node
    void WriteInfoSymbolXML(uint8 code, wxXmlNode *node);
};
