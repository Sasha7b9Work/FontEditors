#pragma once
#include "Font/BitmapFont.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)


class FontImporter
{
public:
    static void Import(BitmapFont &font, wxTextFile &file, const wxString &nameFont);

    static const char *FontWeightToChar(const wxFont &font);
    static const char *FontStyleToChar(const wxFont &font);
    static const char *FontFamilyToChar(const wxFont &font);

    static wxFontWeight FontWeightToENUM(const char *weight);
    static wxFontStyle FontStyleToENUM(const char *style);
    static wxFontFamily FontFamilyToENUM(const char *family);
    
private:

    static void CreateSymbols(BitmapFont &font);
    
    // Рассчитать размеры символов
    static void CalculateSizes(int *sizes);

    static void CalculateOffsets(const int sizes[256], uint16 offsets[256]);

    static void WriteFont(wxTextFile &file, const wxString &nameFont, const uint16 offsets[256]);

    static void WriteParametersFont(wxTextFile &file);

    static void DeleteSymbols();

    static int CalculateFullSize();
};
