#include "LineObject.h"

LineObject::LineObject(const wxPoint& start, const wxPoint& end)
    : CanvasObject(start, wxSize(abs(end.x - start.x), abs(end.y - start.y))), m_endPoint(end) {
}

void LineObject::Draw(wxDC& dc) {
    dc.DrawLine(m_position, m_endPoint);
}