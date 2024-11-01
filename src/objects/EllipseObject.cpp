#include "EllipseObject.h"

EllipseObject::EllipseObject(const wxPoint& position, const wxSize& size)
    : CanvasObject(position, size) {
}

void EllipseObject::Draw(wxDC& dc) {
    dc.DrawEllipse(wxRect(m_position, m_size));
}