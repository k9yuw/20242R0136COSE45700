#include "TextObject.h"
#include <wx/dc.h>
#include <cmath>

// 생성자
TextObject::TextObject(const wxPoint& position, const wxSize& size, const std::string& text, int fontSize)
    : CanvasObject(position, size), m_text(text), fontSize(fontSize) {
    // SetSize(size); // 이미 기본 클래스에서 크기가 설정되므로 필요 없음
}

void TextObject::Draw(wxDC& dc) {
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(wxFont(fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(m_text, m_position.x, m_position.y);
}

void TextObject::SetText(const std::string& newText) {
    m_text = newText;
    // 텍스트의 길이에 따라 크기 조정
    int newWidth = static_cast<int>(m_text.length() * fontSize * 0.6); // 대략적인 폭 계산
    int newHeight = fontSize + 10; // 대략적인 높이 계산
    SetSize(wxSize(newWidth, newHeight));
}

const std::string& TextObject::GetText() const {
    return m_text;
}

void TextObject::SetFontSize(int newSize) {
    fontSize = newSize;
    // 글꼴 크기에 따라 크기 조정
    int newWidth = static_cast<int>(m_text.length() * fontSize * 0.6); // 대략적인 폭 계산
    int newHeight = fontSize + 10; // 대략적인 높이 계산
    SetSize(wxSize(newWidth, newHeight));
}

int TextObject::GetFontSize() const {
    return fontSize;
}
