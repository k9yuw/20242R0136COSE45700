#include "TextObject.h"
#include <wx/dc.h>
#include <cmath>

TextObject::TextObject(const wxPoint& position, const std::string& text, int fontSize)
    : CanvasObject(position, wxSize(text.length() * fontSize, fontSize)), text(text), fontSize(fontSize) {
    // Set the object's size based on the text length and font size
    SetSize(wxSize(text.length() * fontSize, fontSize));
}

void TextObject::Draw(wxDC& dc) {
    dc.SetFont(wxFont(fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(text, m_position.x, m_position.y);
}

bool TextObject::IsMouseInside(int mouseX, int mouseY) const {
    // Check if the mouse is within the bounding box of the text
    return mouseX >= m_position.x && mouseX <= m_position.x + m_size.GetWidth() &&
           mouseY >= m_position.y && mouseY <= m_position.y + fontSize;
}

bool TextObject::IsMouseNearEdge(int mouseX, int mouseY) const {
    // Define a threshold for how close is considered "near"
    int edgeThreshold = 5;

    // Check if the mouse is near any of the four edges
    bool nearLeft = std::abs(mouseX - m_position.x) <= edgeThreshold;
    bool nearRight = std::abs(mouseX - (m_position.x + m_size.GetWidth())) <= edgeThreshold;
    bool nearTop = std::abs(mouseY - m_position.y) <= edgeThreshold;
    bool nearBottom = std::abs(mouseY - (m_position.y + fontSize)) <= edgeThreshold;

    return nearLeft || nearRight || nearTop || nearBottom;
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