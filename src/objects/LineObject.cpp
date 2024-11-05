#include "LineObject.h"

LineObject::LineObject(const wxPoint& start, const wxPoint& end)
    : CanvasObject(start, wxSize(abs(end.x - start.x), abs(end.y - start.y))), m_endPoint(end) {
}

void LineObject::Draw(wxDC& dc) {
    dc.DrawLine(m_position, m_endPoint);
}

bool LineObject::isMouseInside(const wxPoint& point) const {
    int left = std::min(m_position.x, m_endPoint.x);
    int top = std::min(m_position.y, m_endPoint.y);
    int right = std::max(m_position.x, m_endPoint.x);
    int bottom = std::max(m_position.y, m_endPoint.y);

    wxRect boundingRect(left, top, right - left, bottom - top);

    return boundingRect.Contains(point);
}

bool LineObject::isMouseNearEdge(const wxPoint& point, int margin) const {
    wxRect startRect(m_position.x - margin / 2, m_position.y - margin / 2, margin, margin);
    wxRect endRect(m_endPoint.x - margin / 2, m_endPoint.y - margin / 2, margin, margin);

    return startRect.Contains(point) || endRect.Contains(point);
}

const wxPoint& LineObject::GetEndPoint() const {
    return m_endPoint;
}

void LineObject::SetPosition(const wxPoint& pos) { 
    wxPoint delta = pos - m_position;
    m_position = pos;
    m_endPoint += delta;
}

void LineObject::SetStartPoint(const wxPoint& start) {
    m_position = start;
}

void LineObject::SetEndPoint(const wxPoint& end) {
    m_endPoint = end;
}
