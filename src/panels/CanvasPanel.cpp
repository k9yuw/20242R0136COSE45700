#include "CanvasPanel.h"
#include "PropertyPanel.h"
wxBEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_LEFT_DOWN(CanvasPanel::OnMouseClickStart)  // 마우스 왼쪽 버튼 눌림
    EVT_MOTION(CanvasPanel::OnMouseMove)           // 마우스 이동 (이동 또는 크기 조정)
    EVT_LEFT_UP(CanvasPanel::OnMouseClickEnd)      // 마우스 왼쪽 버튼 뗌
    EVT_PAINT(CanvasPanel::OnPaint)                // 화면 갱신
    EVT_LEFT_DCLICK(CanvasPanel::OnLeftDClick)
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
    Update();                  
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
    m_mouseLastPosition = event.GetPosition();   

    wxPoint pos = event.GetPosition();  

    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        // 객체 모서리 근처에서 클릭한 경우: 크기 조정 모드
        if ((*it)->isMouseNearEdge(pos, 20)) { 
            m_selectedObject = *it;
            m_isResizing = true; 
            m_originalPosition = m_selectedObject->GetPosition();
            m_originalSize = m_selectedObject->GetSize();  // 객체의 원래 크기 저장
            m_resizeStartPos = pos;
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
        wxPoint delta = currentPos - m_resizeStartPos; // 마우스 이동량 계산

        // 새 크기 계산 (원래 크기에 delta를 더함)
        wxSize newSize = m_originalSize;
        newSize.SetWidth(m_originalSize.GetWidth() + delta.x);
        newSize.SetHeight(m_originalSize.GetHeight() + delta.y);

        // 최소 크기 제한 (예: 너비와 높이는 최소 10 이상)
        if (newSize.GetWidth() < 10) newSize.SetWidth(10);
        if (newSize.GetHeight() < 10) newSize.SetHeight(10);

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
void CanvasPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this); 
    for (CanvasObject* object : m_objects) {
        object->Draw(dc);  
    }
}

void CanvasPanel::OnLeftDClick(wxMouseEvent& event) {
    wxPoint pos = event.GetPosition();

    // 마우스 위치에 있는 객체를 찾음
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        if ((*it)->isMouseInside(pos)) {
            TextObject* textObject = dynamic_cast<TextObject*>(*it);
            if (textObject) {
                // 텍스트 입력 대화 상자를 열어 사용자가 텍스트를 입력할 수 있게 함
                wxTextEntryDialog dialog(this, "텍스트를 입력하세요:", "텍스트 수정", textObject->GetText());

                if (dialog.ShowModal() == wxID_OK) {
                    // 사용자 입력값으로 텍스트 업데이트
                    textObject->SetText(dialog.GetValue().ToStdString());
                    Refresh(); 
                }
                return; 
            }
        }
    }
}
