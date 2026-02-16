#pragma once


class wxString;
class wxXmlNode;


class XML
{
public:

    // Возвращает потомка node с именем name, если таковой существует
    static wxXmlNode *FindChildren(const wxXmlNode *node, const wxString &name);
};
