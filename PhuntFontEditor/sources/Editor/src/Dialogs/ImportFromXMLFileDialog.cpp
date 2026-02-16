#include "defines.h"
#include "Canvas.h"
#include "XML.h"
#include "Dialogs/ImportFromXMLFileDialog.h"
#include "Font/FontImporter.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/xml/xml.h>
#pragma warning(pop)


bool ImportFromXMLFileDialog::Execute()
{
    wxFileDialog openDialog(nullptr, wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.xml"), wxFD_OPEN);

    if(openDialog.ShowModal() == wxID_OK)
    {
        wxString fileName = openDialog.GetPath();

        wxXmlDocument xml;
        if(xml.Load(fileName))
        {
            wxXmlNode *root = xml.GetRoot();

            wxXmlNode *common = XML::FindChildren(root, "Common");

            if(common)
            {
                long pointSize = 0;
                common->GetAttribute("PointSize").ToLong(&pointSize);
                wxFontFamily fontFamily = FontImporter::FontFamilyToENUM(common->GetAttribute("FontFamily"));
                wxFontStyle fontStyle = FontImporter::FontStyleToENUM(common->GetAttribute("FontStyle"));
                wxFontWeight fontWeigth = FontImporter::FontWeightToENUM(common->GetAttribute("FontWeight"));
                wxString faceName = common->GetAttribute("FaceName");

                SettingsFont settings;

                settings.font.Create(pointSize, fontFamily, fontStyle, fontWeigth, false, faceName);

                wxXmlNode *cell = XML::FindChildren(common, "Cell");

                cell->GetAttribute("Width").ToLong(reinterpret_cast<long *>(&settings.width));
                cell->GetAttribute("Height").ToLong(reinterpret_cast<long *>(&settings.height));
                cell->GetAttribute("OffsetX").ToLong(reinterpret_cast<long *>(&settings.offsetX));
                cell->GetAttribute("OffsetY").ToLong(reinterpret_cast<long *>(&settings.offsetY));

                ImportSystemFontDialog::SetSettingsFont(settings);

                wxXmlNode *symbols = XML::FindChildren(common, "Symbols");

                for(int i = 0; i < 256; i++)
                {
                    char buffer[30];
                    std::sprintf(buffer, "Symbol%d", i);

                    wxXmlNode *symbol = XML::FindChildren(symbols, buffer);

                    wxString enabled = symbol->GetAttribute("Enabled");

                    if(std::strcmp(enabled.c_str(), "0") == 0) //-V2513
                    {
                        TheCanvas->GetFont()->GetSymbol(static_cast<uint8>(i))->Disable();
                    }
                }

                TheCanvas->Rebuild();

                for (int i = 0; i < 256; i++)
                {
                    char buffer[30];
                    std::sprintf(buffer, "Symbol%d", i);

                    wxXmlNode *symbol = XML::FindChildren(symbols, buffer);

                    wxString edited = symbol->GetAttribute("Edited");

                    if (std::strcmp(edited.c_str(), "1") == 0) //-V2513
                    {
                        wxXmlNode *rows = XML::FindChildren(symbol, "Rows");

                        BitmapSymbol *bmpSymbol = TheCanvas->GetFont()->GetSymbol((uint8)i); //-V2533

                        if (rows)
                        {
                            for (int r = 0; r < settings.height; r++)
                            {
                                std::sprintf(buffer, "row%02d", r);

                                wxXmlNode *row = XML::FindChildren(rows, buffer);

                                if (row)
                                {
                                    for (int c = 0; c < settings.width; c++)
                                    {
                                        std::sprintf(buffer, "_%02d", c);

                                        wxString pixel = row->GetAttribute(buffer);

                                        if (std::strcmp(pixel, "0") == 0) //-V2513
                                        {
                                            bmpSymbol->ClearPixel(c, r);
                                        }
                                        else
                                        {
                                            bmpSymbol->SetPixel(c, r);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                TheCanvas->ZoomUp();
                TheCanvas->ZoomDown();
            }
        }
    }

    return true;
}
