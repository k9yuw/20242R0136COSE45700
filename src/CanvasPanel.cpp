#include "CanvasPanel.h"

wxBEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_PAINT(CanvasPanel::OnPaint)                // 그리기 이벤트
    EVT_LEFT_DOWN(CanvasPanel::OnMouseClickStart)  // 마우스 왼쪽 버튼 눌림
    EVT_MOTION(CanvasPanel::OnMouseMove)           // 마우스 이동
    EVT_LEFT_UP(CanvasPanel::OnMouseClickEnd)      // 마우스 왼쪽 버튼 뗌
wxEND_EVENT_TABLE()

// 생성자
CanvasPanel::CanvasPanel(wxWindow* parentWindow)
    : wxPanel(parentWindow), isMouseDrawing(false), m_selectedObject(nullptr) {
    // 캔버스 초기화
    SetBackgroundColour(*wxWHITE);
    SetMinSize(wxSize(400, 300));
}

// object 추가
void CanvasPanel::AddObject(CanvasObject* object) {
    m_objects.push_back(object); 
    Refresh();                  
}

// object 반환
CanvasObject* CanvasPanel::GetSelectedObject() const {
    return m_selectedObject;
}

// object 리스트 반환
const std::vector<CanvasObject*>& CanvasPanel::GetObjects() const {
    return m_objects;
}

// 선택된 object 설정
void CanvasPanel::SetSelectedObject(CanvasObject* object) {
    m_selectedObject = object;
    Refresh();
}

// 화면 갱신
void CanvasPanel::RefreshCanvas() {
    Refresh();  
}

// 그리기 이벤트핸들러
void CanvasPanel::OnPaint(wxPaintEvent& event) {

    wxPaintDC dc(this);
    for (auto object : m_objects) {
        object->Draw(dc);
    }

    // 현재 마우스 드로잉 상태일 때
    if (isMouseDrawing) {
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(mouseLastPoint, ScreenToClient(wxGetMousePosition()));
    }
}

void CanvasPanel::OnMouseClickStart(wxMouseEvent& event) {
    isMouseDrawing = true;                  
    mouseLastPoint = event.GetPosition();  

    // 마우스 위치에서 객체를 찾기
    wxPoint pos = event.GetPosition();
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        if ((*it)->isMouseInside(pos)) {
            m_selectedObject = *it; 
            Refresh();               
            return;
        }
    }
    m_selectedObject = nullptr; 
    Refresh();
}

// 마우스 이동 이벤트 핸들러
void CanvasPanel::OnMouseMove(wxMouseEvent& event) {
    if (isMouseDrawing && event.Dragging() && event.LeftIsDown()) {
        wxClientDC dc(this); 
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(mouseLastPoint, event.GetPosition()); 
        mouseLastPoint = event.GetPosition();             
    }
}

// 마우스 왼쪽 버튼 뗌 이벤트 핸들러 
void CanvasPanel::OnMouseClickEnd(wxMouseEvent& event) {
    isMouseDrawing = false; 
}
