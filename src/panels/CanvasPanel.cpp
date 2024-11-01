#include "CanvasPanel.h"
#include "PropertyPanel.h"
#include "objects/CanvasObject.h"
#include "objects/TextObject.h" 
#include <algorithm> 

wxBEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_LEFT_DOWN(CanvasPanel::OnMouseClickStart)  // 마우스 왼쪽 버튼 눌림
    EVT_MOTION(CanvasPanel::OnMouseMove)           // 마우스 이동 (이동 또는 크기 조정)
    EVT_LEFT_UP(CanvasPanel::OnMouseClickEnd)      // 마우스 왼쪽 버튼 뗌
    EVT_PAINT(CanvasPanel::OnPaint)                // 화면 갱신
    EVT_LEFT_DCLICK(CanvasPanel::OnLeftDClick)     // 마우스 왼쪽 버튼 더블 클릭
wxEND_EVENT_TABLE()

// 생성자
CanvasPanel::CanvasPanel(wxWindow* parentWindow)
    : wxPanel(parentWindow), 
      m_isMouseClicked(false), 
      m_isDragging(false), 
      m_isResizing(false), 
      m_isSelecting(false), 
      m_propertyPanel(nullptr) {
    // 캔버스 초기화
    SetBackgroundColour(*wxWHITE);
    SetMinSize(wxSize(400, 300));
}

// 소멸자
CanvasPanel::~CanvasPanel() {
    for(auto obj : m_objects) {
        delete obj;
    }
    m_objects.clear();
}

// PropertyPanel 설정 함수
void CanvasPanel::SetPropertyPanel(PropertyPanel* propertyPanel) {
    m_propertyPanel = propertyPanel;
}

// 객체 추가
void CanvasPanel::AddObject(CanvasObject* object) {
    m_objects.push_back(object); 
    Refresh();
    Update();                  
}

// 선택된 객체들을 반환
const std::vector<CanvasObject*>& CanvasPanel::GetSelectedObjects() const {
    return m_selectedObjects;
}

// 선택된 객체들을 설정
void CanvasPanel::SetSelectedObjects(const std::vector<CanvasObject*>& objects) {
    m_selectedObjects = objects;

    // 선택된 객체들의 원래 위치 저장
    m_originalPositions.clear();
    for(auto& obj : m_selectedObjects) {
        m_originalPositions.push_back(obj->GetPosition());
    }

    // PropertyPanel에 선택된 객체 전달
    if (m_propertyPanel) {
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    }

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
    bool isCtrlDown = event.CmdDown(); // Mac에서는 Cmd, Windows에서는 Ctrl

    // 객체를 역순으로 탐색하여 최상위 객체부터 선택
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        if ((*it)->isMouseNearEdge(pos, 20)) { 
            bool isSelected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), *it) != m_selectedObjects.end();

            if (!isCtrlDown && !isSelected) {
                m_selectedObjects.clear();
                m_originalPositions.clear();
            }

            // 리사이징을 위한 선택
            if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), *it) == m_selectedObjects.end()) {
                m_selectedObjects.push_back(*it);
                m_originalPositions.push_back((*it)->GetPosition());
            }

            m_isResizing = true; 
            m_originalSize = (*it)->GetSize();
            m_resizeStartPos = pos;

            // PropertyPanel에 선택된 객체 전달
            if (m_propertyPanel) {
                m_propertyPanel->SetSelectedObjects(m_selectedObjects);
            }

            Refresh(); 
            return;
        } 
        else if ((*it)->isMouseInside(pos)) { 
            bool isSelected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), *it) != m_selectedObjects.end();

            if (isCtrlDown) {
                // Ctrl 키가 눌려있으면 토글 선택
                auto it_selected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), *it);
                if (it_selected != m_selectedObjects.end()) {
                    // 이미 선택된 객체면 선택 해제
                    size_t index = std::distance(m_selectedObjects.begin(), it_selected);
                    m_selectedObjects.erase(it_selected);
                    m_originalPositions.erase(m_originalPositions.begin() + index);
                } else {
                    // 선택되지 않은 객체면 선택 추가
                    m_selectedObjects.push_back(*it);
                    m_originalPositions.push_back((*it)->GetPosition());
                }
            } else {
                if (!isSelected) {
                    // Ctrl 키가 눌려있지 않고 클릭한 객체가 선택되지 않은 경우
                    // 기존 선택 해제하고 새로 선택
                    m_selectedObjects.clear();
                    m_originalPositions.clear();
                    m_selectedObjects.push_back(*it);
                    m_originalPositions.push_back((*it)->GetPosition());
                }
                // 만약 클릭한 객체가 이미 선택된 상태라면, 선택을 유지함
                // 따라서 멀티 선택 상태가 유지됨
            }

            m_isDragging = true;  
            m_dragStartPos = pos;

            // PropertyPanel에 선택된 객체 전달
            if (m_propertyPanel) {
                m_propertyPanel->SetSelectedObjects(m_selectedObjects);
            }

            Refresh(); 
            return;
        }
    }

    // 클릭한 위치에 객체가 없고 Ctrl 키가 눌리지 않은 경우
    if (!event.CmdDown()) { // Mac에서는 Cmd, Windows에서는 Ctrl
        m_selectedObjects.clear();
        m_originalPositions.clear();

        // PropertyPanel에 선택 해제 전달
        if (m_propertyPanel) {
            m_propertyPanel->SetSelectedObjects(m_selectedObjects);
        }
    }

    // 드래그로 선택 영역을 시작
    m_isSelecting = true;
    m_selectionStartPos = pos;
    m_selectionEndPos = pos;
    Refresh(); 
}


// 마우스가 움직일 때 호출되는 함수 (이동 또는 크기 조정)
void CanvasPanel::OnMouseMove(wxMouseEvent& event) {
    wxPoint currentPos = event.GetPosition();

    if (m_isSelecting) {
        m_selectionEndPos = currentPos;
        Refresh();
        return;
    }

    // 위치 변경 처리
    if (m_isDragging && !m_isResizing && !m_selectedObjects.empty()) {
        wxPoint delta = currentPos - m_dragStartPos;

        for (size_t i = 0; i < m_selectedObjects.size(); ++i) {
            wxPoint newPosition = m_originalPositions[i] + delta; 
            m_selectedObjects[i]->SetPosition(newPosition);
        }

        Refresh();  
    }

    // 크기 변경 처리
    if (m_isResizing && !m_selectedObjects.empty()) {
        wxPoint delta = currentPos - m_resizeStartPos;
        wxSize newSize = m_originalSize;
        newSize.SetWidth(m_originalSize.GetWidth() + delta.x);
        newSize.SetHeight(m_originalSize.GetHeight() + delta.y);

        if (newSize.GetWidth() < 10) newSize.SetWidth(10);
        if (newSize.GetHeight() < 10) newSize.SetHeight(10);

        for (auto& obj : m_selectedObjects) {
            obj->SetSize(newSize);
        }
        Refresh();  
    }

    // 커서 변경: 리사이징 가능한지 확인
    if (!m_isResizing && !m_isDragging && !m_isSelecting) {
        bool onEdge = false;
        for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
            if ((*it)->isMouseNearEdge(currentPos, 20)) { 
                SetCursor(wxCURSOR_SIZENWSE);
                onEdge = true;
                break;
            }
        }
        if (!onEdge) {
            SetCursor(wxCURSOR_ARROW);
        }
    }
}

// 마우스 버튼을 뗐을 때 호출되는 함수
void CanvasPanel::OnMouseClickEnd(wxMouseEvent& event) {
    m_isMouseClicked = false; 
    m_isDragging = false; 
    m_isResizing = false; 

    // 선택 영역 드래그가 끝났을 때
    if (m_isSelecting) {
        m_isSelecting = false;

        int left = std::min(m_selectionStartPos.x, m_selectionEndPos.x);
        int top = std::min(m_selectionStartPos.y, m_selectionEndPos.y);
        int right = std::max(m_selectionStartPos.x, m_selectionEndPos.x);
        int bottom = std::max(m_selectionStartPos.y, m_selectionEndPos.y);

        wxRect selectionRect(left, top, right - left, bottom - top);

        m_selectedObjects.clear();
        m_originalPositions.clear();

        // 선택 영역 내에 있는 모든 객체를 선택
        for (auto& obj : m_objects) {
            wxRect objRect(obj->GetPosition(), obj->GetSize());
            if (selectionRect.Intersects(objRect)) {
                m_selectedObjects.push_back(obj);
                m_originalPositions.push_back(obj->GetPosition());
            }
        }

        // PropertyPanel에 선택된 객체 전달
        if (m_propertyPanel) {
            m_propertyPanel->SetSelectedObjects(m_selectedObjects);
        }

        Refresh();
    }
}

// 화면 갱신
void CanvasPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this); 
    for (CanvasObject* object : m_objects) {
        object->Draw(dc);  
    }

    // 선택된 객체에 테두리 그리기
    for (CanvasObject* obj : m_selectedObjects) {
        wxRect rect(obj->GetPosition(), obj->GetSize());
        wxRect paddingRect = rect;
        paddingRect.Inflate(5, 5); // 간격을 두기 위해 인플레이트

        dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_SOLID));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(paddingRect);
    }

    // 선택 영역 드래그 중인 경우 선택 영역 그리기
    if (m_isSelecting) {
        int left = std::min(m_selectionStartPos.x, m_selectionEndPos.x);
        int top = std::min(m_selectionStartPos.y, m_selectionEndPos.y);
        int right = std::max(m_selectionStartPos.x, m_selectionEndPos.x);
        int bottom = std::max(m_selectionStartPos.y, m_selectionEndPos.y);

        wxRect selectionRect(left, top, right - left, bottom - top);

        dc.SetPen(wxPen(wxColour(0, 120, 215), 1, wxPENSTYLE_SHORT_DASH));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(selectionRect);
    }
}

// 텍스트 객체 더블 클릭 시 텍스트 수정
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
