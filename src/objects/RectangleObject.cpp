#include "RectangleObject.h"

RectangleObject::RectangleObject(const wxPoint& position, const wxSize& size)
    : CanvasObject(position, size) {
}

void RectangleObject::Draw(wxDC& dc) {
    dc.DrawRectangle(wxRect(m_position, m_size));
}