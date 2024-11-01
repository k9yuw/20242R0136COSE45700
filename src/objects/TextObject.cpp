#include "TextObject.h"
#include <wx/dc.h>
#include <cmath>

TextObject::TextObject(const wxPoint& position, const std::string& text, int fontSize)
    : CanvasObject(position, wxSize(text.length() * fontSize, fontSize)), text(text), fontSize(fontSize) {
    SetSize(wxSize(text.length() * fontSize, fontSize));
}

void TextObject::Draw(wxDC& dc) {
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(text, m_position.x, m_position.y);
}

void TextObject::SetText(const std::string& newText) {
    text = newText;
    SetSize(wxSize(text.length() * fontSize, fontSize));
}

const std::string& TextObject::GetText() const {
    return text;
}

void TextObject::SetFontSize(int newSize) {
    fontSize = newSize;
    SetSize(wxSize(text.length() * fontSize, fontSize));
}

int TextObject::GetFontSize() const {
    return fontSize;
}