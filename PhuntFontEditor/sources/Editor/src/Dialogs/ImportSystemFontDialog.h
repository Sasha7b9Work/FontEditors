#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct SettingsFont
{
    int width;      // Ширина знкоместа
    int height;     // Высота знакоместа
    int offsetX;    // Смещение шрифта по горизонтали
    int offsetY;    // Смещение шрифта по вертикали
    wxFont font;    // Импортируемый шрифт  
};


class ImportSystemFontDialog : public wxDialog
{
public:
    ImportSystemFontDialog(const wxString &title);

    static SettingsFont GetSettingsFont();

    static void SetSettingsFont(const SettingsFont &settings);
   
protected:

    void OnChoiceFont(wxCommandEvent &);

private:

    void OnButtonApply(wxCommandEvent &);

    void OnButtonClose(wxCommandEvent &);
    
    // Сформировать строку описания шрифта
    static void TuneTextFont();
    
    // Установить все полагающиеся надписи в соотвествии с текущими настройками
    static void TuneTexts();

    static SettingsFont settings;       // Настройки шрифта
};
