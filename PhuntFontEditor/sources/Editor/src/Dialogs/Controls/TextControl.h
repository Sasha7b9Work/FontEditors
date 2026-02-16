#pragma once


class TextControl : public wxPanel
{
public:
    TextControl(wxWindow *parent, const wxString &value, const wxString &label);

    int ToInt();

    void WriteInt(int value);

    wxString GetLineText();

private:
    wxTextCtrl *tc = nullptr;
};
