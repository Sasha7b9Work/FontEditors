#include "defines.h"
#include "Canvas.h"
#include "Dialogs/ImportSystemFontDialog.h"
#include "Dialogs/Controls/TextControl.h"
#pragma warning(push, 0)
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_BUTTON_APPLY,
    ID_BUTTON_CLOSE,
    ID_BUTTON_FONT
};


SettingsFont ImportSystemFontDialog::settings = { 16, 16, 0, 0, wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New")) };

// Информация о выбранном шрифте
static wxStaticText *textFont = nullptr;

static TextControl *tcWidthCell = nullptr;
static TextControl *tcHeightCell = nullptr;
static TextControl *tcOffsetX = nullptr;
static TextControl *tcOffsetY = nullptr;


ImportSystemFontDialog::ImportSystemFontDialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
#define BORDER 5
#define SPACER 10
#define FULL_ALIGN wxALIGN_CENTER | wxALL


    wxButton *btnApply = new wxButton(this, ID_BUTTON_APPLY, wxT("Применить"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(ID_BUTTON_APPLY, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnButtonApply));
    wxButton *btnClose = new wxButton(this, ID_BUTTON_CLOSE, wxT("Закрыть"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(ID_BUTTON_CLOSE, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnButtonClose));
    wxBoxSizer *boxButtons = new wxBoxSizer(wxHORIZONTAL); //-V2511
    boxButtons->Add(btnApply, 1, wxALIGN_CENTER);
    boxButtons->AddSpacer(20);
    boxButtons->Add(btnClose, 1, wxALIGN_CENTER);

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL); //-V2511

    wxBoxSizer *boxFont = new wxBoxSizer(wxHORIZONTAL); //-V2511
    wxButton *btnFont = new wxButton(this, ID_BUTTON_FONT, wxT("Шрифт"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(ID_BUTTON_FONT, wxEVT_BUTTON, wxCommandEventHandler(ImportSystemFontDialog::OnChoiceFont));
    textFont = new wxStaticText(this, wxID_ANY, ""); //-V2511
    boxFont->Add(btnFont, wxALIGN_LEFT, BORDER);
    boxFont->AddSpacer(SPACER);
    boxFont->Add(textFont, 0, wxALIGN_CENTER_VERTICAL, BORDER);

    tcWidthCell = new TextControl(this, wxT("8"), wxT("ширина символа")); //-V2511

    tcHeightCell = new TextControl(this, wxT("8"), wxT("высота символа")); //-V2511

    tcOffsetX = new TextControl(this, wxT("0"), wxT("смещение по x")); //-V2511

    tcOffsetY = new TextControl(this, wxT("0"), wxT("смещение по y")); //-V2511

    vBox->Add(boxFont, 0, wxALIGN_LEFT, BORDER);
    vBox->Add(tcWidthCell, 0, FULL_ALIGN, BORDER);
    vBox->Add(tcHeightCell, 0, FULL_ALIGN, BORDER);
    vBox->Add(tcOffsetX, 0, FULL_ALIGN, BORDER);
    vBox->Add(tcOffsetY, 0, FULL_ALIGN, BORDER);

    vBox->AddSpacer(10);
    vBox->Add(boxButtons, 0, FULL_ALIGN, BORDER);

    SetSizer(vBox);

    TuneTexts();
}


void ImportSystemFontDialog::OnChoiceFont(wxCommandEvent &)
{
    wxFontDialog dlg(this);
    
    if (dlg.ShowModal() == wxID_OK)
    {
        settings.font = dlg.GetFontData().GetChosenFont();

        TuneTextFont();
    }
}


void ImportSystemFontDialog::OnButtonApply(wxCommandEvent &)
{
    settings.width = tcWidthCell->ToInt();
    settings.height = tcHeightCell->ToInt();
    settings.offsetX = tcOffsetX->ToInt();
    settings.offsetY = tcOffsetY->ToInt();

    TheCanvas->Rebuild();
}


void ImportSystemFontDialog::OnButtonClose(wxCommandEvent &)
{
    Destroy();
}


SettingsFont ImportSystemFontDialog::GetSettingsFont()
{
    return settings;
}

void ImportSystemFontDialog::SetSettingsFont(const SettingsFont &set)
{
    settings = set;
}


void ImportSystemFontDialog::TuneTexts()
{
    TuneTextFont();

    tcWidthCell->WriteInt(settings.width);
    tcHeightCell->WriteInt(settings.height);
    tcOffsetX->WriteInt(settings.offsetX);
    tcOffsetY->WriteInt(settings.offsetY);
}


void ImportSystemFontDialog::TuneTextFont()
{
    int size = settings.font.GetPointSize();

    settings.font.SetPointSize(10);

    textFont->SetFont(settings.font);

    settings.font.SetPointSize(size);

    textFont->SetLabel(wxString::Format("%s %d %s", settings.font.GetFaceName(), settings.font.GetPointSize(), wxT("Пример")));
}
