#include "Dialogs/Controls/TextControl.h"


TextControl::TextControl(wxWindow *parent, const wxString &value, const wxString &label) : wxPanel(parent)
{
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL); //-V2511

    tc = new wxTextCtrl(this, wxID_ANY, value, wxDefaultPosition, { 50, 20 }); //-V2511
    tc->SetMinSize({ 100, 20 });
    box->Add(tc, wxALIGN_CENTER | wxALL);
    box->AddSpacer(10);
    box->Add(new wxStaticText(this, wxID_ANY, label), wxALIGN_CENTER | wxALL, 5); //-V2511

    SetSizer(box);
}


int TextControl::ToInt()
{
    long l = 0;

    tc->GetLineText(0).ToLong(&l);

    return static_cast<int>(l);
}


void TextControl::WriteInt(int value)
{
    tc->Clear();
    tc->WriteText(wxString::Format("%i", value));
}


wxString TextControl::GetLineText()
{
    return tc->GetLineText(0);
}
