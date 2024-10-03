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
    // 변경 시 콜백 함수 호출
    if(m_onChangeCallback) {
        m_onChangeCallback();
    }
}

wxSize CanvasObject::GetSize() const {
    return m_size;
}

void CanvasObject::SetSize(const wxSize& size) {
    m_size = size;
    // 변경 시 콜백 함수 호출
    if(m_onChangeCallback) {
        m_onChangeCallback();
    }
}

int CanvasObject::GetZOrder() const {
    return m_zOrder;
}

void CanvasObject::SetZOrder(int zOrder) {
    m_zOrder = zOrder;
    // 변경 시 콜백 함수 호출
    if(m_onChangeCallback) {
        m_onChangeCallback();
    }
}

// SetOnChangeCallBack은 m_onChangeCallback 멤버변수에 인자로 들어온 callback을 등록시키는 역할
// 나중에 SetPosition이나 SetSize 등에서 m_onChangeCallback을 호출
void CanvasObject::SetOnChangeCallback(std::function<void()> callback) {
    m_onChangeCallback = callback;  // 외부에서 콜백 등록
}