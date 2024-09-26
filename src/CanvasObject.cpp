#include "CanvasObject.h"

CanvasObject::CanvasObject(const wxPoint& position, const wxSize& size)
    : m_position(position), m_size(size), m_zOrder(0) {}

bool CanvasObject::isMouseInside(const wxPoint& point) const {
    // 객체의 위치와 크기로부터 사각형 영역을 만들고,
    // 주어진 점이 사각형 영역 안에 있는지 확인한다.
    wxRect rect(m_position, m_size);
    return rect.Contains(point);
}

wxPoint CanvasObject::GetPosition() const {
    return m_position;
}

void CanvasObject::SetPosition(const wxPoint& position) {
    m_position = position;
}

wxSize CanvasObject::GetSize() const {
    return m_size;
}

void CanvasObject::SetSize(const wxSize& size) {
    m_size = size;
}

int CanvasObject::GetZOrder() const {
    return m_zOrder;
}

void CanvasObject::SetZOrder(int zOrder) {
    m_zOrder = zOrder;
}
