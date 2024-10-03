#include "CanvasPanel.h"

wxBEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_LEFT_DOWN(CanvasPanel::OnMouseClickStart)  // 마우스 왼쪽 버튼 눌림
    EVT_MOTION(CanvasPanel::OnMouseMove)           // 마우스 이동 (이동 또는 크기 조정)
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


// 마우스가 클릭될 때 호출되는 함수
void CanvasPanel::OnMouseClickStart(wxMouseEvent& event) {
    m_isMouseClicked = true;                  
    m_mouseLastPoint = event.GetPosition();   

    wxPoint pos = event.GetPosition();  

    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        // 객체 모서리 근처에서 클릭한 경우: 크기 조정 모드
        if ((*it)->isMouseNearEdge(pos, 10)) { 
            m_selectedObject = *it;
            m_isResizing = true; 
            m_originalPosition = m_selectedObject->GetPosition();
            m_originalSize = m_selectedObject->GetSize();  // 객체의 원래 크기 저장
            return;
        }
        // 객체 내부에서 클릭한 경우: 이동 모드
        else if ((*it)->isMouseInside(pos)) { 
            m_selectedObject = *it;
            m_isDragging = true;  
            m_dragStartPos = pos;
            m_originalPosition = m_selectedObject->GetPosition();
            return;
        }
    }

    m_selectedObject = nullptr;  // 객체가 없으면 선택된 객체를 null로 설정
    Refresh(); 
}

// 마우스가 움직일 때 호출되는 함수 (이동 또는 크기 조정)
void CanvasPanel::OnMouseMove(wxMouseEvent& event) {
    // 위치 변경 처리
    if (m_isDragging && m_selectedObject) {
        wxPoint currentPos = event.GetPosition(); 
        wxPoint newPosition = m_originalPosition + (currentPos - m_dragStartPos);  // 마우스 이동량에 따라 새 위치 계산
        m_selectedObject->SetPosition(newPosition);  
        Refresh();  
    }

    // 크기 변경 처리
    if (m_isResizing && m_selectedObject) {
        wxPoint currentPos = event.GetPosition();
        wxSize newSize = m_originalSize;
        newSize.SetWidth(currentPos.x - m_originalPosition.x);  // 새 너비 설정
        newSize.SetHeight(currentPos.y - m_originalPosition.y);  // 새 높이 설정
        m_selectedObject->SetSize(newSize);  // 객체 크기 업데이트
        Refresh();  
    }
}

// 마우스 버튼을 뗐을 때 호출되는 함수
void CanvasPanel::OnMouseClickEnd(wxMouseEvent& event) {
    m_isMouseClicked = false; 
    m_isDragging = false; 
    m_isResizing = false; 
}
