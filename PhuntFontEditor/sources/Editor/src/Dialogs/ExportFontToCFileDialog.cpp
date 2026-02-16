#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ExportFontToCFileDialog.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Dialogs/Controls/TextControl.h"
#include "Font/FontImporter.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#include <wx/xml/xml.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_EXPORT,
    ID_BUTTON_CANCEL
};


static TextControl *tcNameFont = nullptr;
static wxString nameFont(wxT("font"));


ExportFontToCFileDialog::ExportFontToCFileDialog() : wxDialog(nullptr, wxID_ANY, wxT("Экспорт шрифта в Си-файл"))
{
#define BORDER 5
#define SPACER 10
#define FULL_ALIGN wxALIGN_CENTER | wxALL

    wxButton *btnExport = new wxButton(this, ID_BUTTON_EXPORT, wxT("Экспорт"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(ID_BUTTON_EXPORT, wxEVT_BUTTON, wxCommandEventHandler(ExportFontToCFileDialog::OnButtonExport));
    wxButton *btnCancel = new wxButton(this, ID_BUTTON_CANCEL, wxT("Отмена"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(ID_BUTTON_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(ExportFontToCFileDialog::OnButtonCancel));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL); //-V2511
    boxButtons->Add(btnExport, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnCancel, 1, wxALIGN_CENTER);

    tcNameFont = new TextControl(this, nameFont, wxT("Имя шрифта")); //-V2511
    tcNameFont->SetMinSize({ 200, 20 });

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL); //-V2511
    
    vBox->Add(tcNameFont);
    vBox->Add(boxButtons, 0, FULL_ALIGN, BORDER);

    SetSizer(vBox);
}


void ExportFontToCFileDialog::OnButtonExport(wxCommandEvent &) //-V2506
{
    nameFont = tcNameFont->GetLineText();

    char nameFile[100] = { 0 };
    std::strcpy(nameFile, nameFont.c_str()); //-V2513
    std::strcat(nameFile, ".inc"); //-V2513

    wxFileDialog dlg(nullptr, wxT("Экспорт"), wxEmptyString, nameFile, wxT("*.inc"), wxFD_SAVE);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxTextFile file(dlg.GetPath());

        if(file.Exists())
        {
            wxMessageDialog message(this, wxT("Файл с таким именем уже существует. Перезаписать?"), wxMessageBoxCaptionStr, wxOK | wxCANCEL | wxCENTRE);
            if(message.ShowModal() == wxID_CANCEL)
            {
                return;
            }
        }

        file.Create();

        TheCanvas->ImportFont(file, nameFont);

        file.Write();

        file.Close();

        Destroy();

        WriteFileXML(file.GetName());
    }
}


void ExportFontToCFileDialog::WriteFileXML(const wxString &nameFileFont)
{
    SettingsFont set = ImportSystemFontDialog::GetSettingsFont();

    wxString nameFile = nameFileFont;
    nameFile[nameFile.size() - 1] = 'l';
    nameFile[nameFile.size() - 2] = 'm';
    nameFile[nameFile.size() - 3] = 'x';

    wxXmlDocument xml;

    wxXmlNode *root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, _T("FontProperties")); //-V2511
    wxXmlNode *common = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, _T("Common")); //-V2511
    root->AddChild(common);

    common->AddAttribute(_T("FaceName"), set.font.GetFaceName());
    common->AddAttribute(_T("PointSize"), wxString::Format(wxT("%i"), set.font.GetPointSize()));
    common->AddAttribute(_T("FontWeight"), FontImporter::FontWeightToChar(set.font));
    common->AddAttribute(_T("FontStyle"), FontImporter::FontStyleToChar(set.font));
    common->AddAttribute(_T("FontFamily"), FontImporter::FontFamilyToChar(set.font));

    wxXmlNode *cell = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, _T("Cell")); //-V2511
    common->AddChild(cell);

    cell->AddAttribute(_T("Width"), wxString::Format(wxT("%i"), set.width));
    cell->AddAttribute(_T("Height"), wxString::Format(wxT("%i"), set.height));
    cell->AddAttribute(_T("OffsetX"), wxString::Format(wxT("%i"), set.offsetX));
    cell->AddAttribute(_T("OffsetY"), wxString::Format(wxT("%i"), set.offsetY));

    wxXmlNode *symbols = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, _T("Symbols")); //-V2511
    common->AddChild(symbols);

    for (int i = 0; i < 256; i++)
    {
        WriteInfoSymbolXML(static_cast<uint8>(i), symbols);
    }

    xml.SetRoot(root);

    xml.Save(nameFile, 2);
}


void ExportFontToCFileDialog::WriteInfoSymbolXML(uint8 code, wxXmlNode *node)
{
    BitmapSymbol *symbol = TheCanvas->GetFont()->GetSymbol(code);

    wxXmlNode *nodeSymbol = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, wxString::Format(wxT("Symbol%d"), code)); //-V2511
    node->AddChild(nodeSymbol);

    nodeSymbol->AddAttribute(_T("Enabled"), symbol->IsEnabled() ? "1" : "0");

    nodeSymbol->AddAttribute(_T("Edited"), symbol->IsEdited() ? "1" : "0");

    if (symbol->IsEdited())
    {
        wxXmlNode *rows = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, _T("Rows")); //-V2511
        nodeSymbol->AddChild(rows);

        SettingsFont set = ImportSystemFontDialog::GetSettingsFont();

        for (int r = 0; r < set.height; r++)
        {
            wxXmlNode *row = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, wxString::Format(wxT("row%02d"), r)); //-V2511
            rows->AddChild(row);

            for (int c = 0; c < set.width; c++)
            {
                row->AddAttribute(wxString::Format(wxT("_%02d"), c), symbol->GetPixel(c, r) ? "0" : "1");
            }
        }
    }
}


void ExportFontToCFileDialog::OnButtonCancel(wxCommandEvent &)
{
    Destroy();
}
