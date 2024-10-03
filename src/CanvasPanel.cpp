#include "CanvasPanel.h"

wxBEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_PAINT(CanvasPanel::OnPaint)                // 그리기 이벤트
    EVT_LEFT_DOWN(CanvasPanel::OnMouseClickStart)  // 마우스 왼쪽 버튼 눌림
    EVT_MOTION(CanvasPanel::OnObjectMove)           // 마우스 이동
    EVT_LEFT_UP(CanvasPanel::OnMouseClickEnd)      // 마우스 왼쪽 버튼 뗌
wxEND_EVENT_TABLE()

// 생성자
CanvasPanel::CanvasPanel(wxWindow* parentWindow)
    : wxPanel(parentWindow), m_isMouseClicked(false), m_isDragging(false), m_selectedObject(nullptr) {
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
    if (m_isMouseClicked) {
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(m_mouseLastPoint, ScreenToClient(wxGetMousePosition()));
    }
}

void CanvasPanel::OnMouseClickStart(wxMouseEvent& event) {
    m_isMouseClicked = true;                  
    m_mouseLastPoint = event.GetPosition();   

    // 마우스 위치에서 객체를 찾기
    wxPoint pos = event.GetPosition();  
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) { // objects 벡터의 마지막 객체부터 시작해서 첫 번째 객체까지 역순으로 순회
        if ((*it)->isMouseInside(pos)) {  // 객체 내부에 마우스 좌표가 있는지 확인
            m_selectedObject = *it;       // 선택된 객체 설정
            m_isDragging = true;          
            m_dragStartPos = pos;         
            m_originalPosition = m_selectedObject->GetPosition(); 
            Refresh();  
            return;
        }
    }
    m_selectedObject = nullptr;  // 객체가 없으면 선택된 객체를 null로 설정
    Refresh(); 
}

// 객체를 드래그할 때 위치 변경을 처리하는 함수
void CanvasPanel::OnObjectMove(wxMouseEvent& event) {
    // 객체 드래그 중일 때 위치 변경 처리
    if (m_isDragging && m_selectedObject) {
        wxPoint currentPos = event.GetPosition();  // 현재 마우스 위치를 가져옴
        wxPoint newPosition = m_originalPosition + (currentPos - m_dragStartPos);  // 마우스 이동량에 따라 새 위치 계산
        m_selectedObject->SetPosition(newPosition);  
        Refresh();  
    }
}

void CanvasPanel::OnMouseClickEnd(wxMouseEvent& event) {
    m_isMouseClicked = false; 
    m_isDragging = false; // 드래그 종료
}
