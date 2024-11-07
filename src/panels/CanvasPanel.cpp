// CanvasPanel.cpp
#include "CanvasPanel.h"
#include "PropertyPanel.h"
#include "objects/CanvasObject.h"
#include "objects/TextObject.h" 
#include "objects/LineObject.h" 
#include <algorithm>

// 이벤트 테이블 설정
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
      m_resizingStartPoint(false), // 초기값 
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
    ReorderObjectsByZOrder();
    Refresh();
    Update();                  
}

// z-order 재정렬 함수
void CanvasPanel::ReorderObjectsByZOrder() {
    std::sort(m_objects.begin(), m_objects.end(), 
        [](const CanvasObject* a, const CanvasObject* b) -> bool {
            return a->GetZOrder() < b->GetZOrder();
        });
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
        wxLogMessage("Setting selected objects in PropertyPanel.");
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    } else {
        wxLogMessage("PropertyPanel is not set in CanvasPanel.");
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

    // 클릭된 객체 찾기
    CanvasObject* clickedObj = GetTopMostObjectAtPosition(pos);

    // 1. 객체가 존재할 때
    if (clickedObj) {
        // 1-1. 가장자리 클릭한 경우: 리사이징 모드
        if (clickedObj->isMouseNearEdge(pos, 10)) {
            HandleEdgeClick(clickedObj, pos, isCtrlDown);
            return;
        }

        // 1-2. 내부를 클릭한 경우
        HandleObjectClick(clickedObj, pos, isCtrlDown);
        return;
    }

    // 2. 빈공간을 클릭한 경우
    HandleEmptySpaceClick(pos, isCtrlDown);
}

// 클릭된 위치에서 최상위 객체를 찾는 함수
CanvasObject* CanvasPanel::GetTopMostObjectAtPosition(const wxPoint& pos) const {
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        if ((*it)->isMouseNearEdge(pos, 10) || (*it)->isMouseInside(pos)) {
            return *it;
        }
    }
    return nullptr;
}

// 객체의 가장자리 클릭을 처리하는 함수 (리사이징)
void CanvasPanel::HandleEdgeClick(CanvasObject* obj, const wxPoint& pos, bool isCtrlDown) {
    bool isSelected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), obj) != m_selectedObjects.end();

    if (!isCtrlDown && !isSelected) {
        m_selectedObjects.clear();
        m_originalPositions.clear();
    }

    // 리사이징을 위한 선택
    if (!isSelected) {
        m_selectedObjects.push_back(obj);
        m_originalPositions.push_back(obj->GetPosition());
    }

    LineObject* lineObj = dynamic_cast<LineObject*>(obj);
    if (lineObj) {
        wxRect startRect(lineObj->GetPosition() - wxPoint(5, 5), wxSize(10, 10));
        wxRect endRect(lineObj->GetEndPoint() - wxPoint(5, 5), wxSize(10, 10));

        if (startRect.Contains(pos)) {
            m_resizingStartPoint = true;
        } else if (endRect.Contains(pos)) {
            m_resizingStartPoint = false;
        }
    }

    m_isResizing = true; 
    m_resizeStartPos = pos;

    // PropertyPanel에 선택된 객체 전달
    if (m_propertyPanel) {
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    }

    Refresh(); 
}

// 객체 클릭을 처리하는 함수 
void CanvasPanel::HandleObjectClick(CanvasObject* obj, const wxPoint& pos, bool isCtrlDown) {
    bool isSelected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), obj) != m_selectedObjects.end();

    if (isCtrlDown) {
        ToggleSelection(obj);
    } else {
        if (!isSelected) {
            m_selectedObjects.clear();
            m_originalPositions.clear();
            m_selectedObjects.push_back(obj);
            m_originalPositions.push_back(obj->GetPosition());
        }
        // 이미 선택된 객체라면 선택을 유지함
    }

    m_isDragging = true;  
    m_dragStartPos = pos;

    // PropertyPanel에 선택된 객체 전달
    if (m_propertyPanel) {
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    }

    Refresh(); 
}

// 빈 공간 클릭을 처리하는 함수 (선택 해제 및 선택 영역 시작)
void CanvasPanel::HandleEmptySpaceClick(const wxPoint& pos, bool isCtrlDown) {
    if (!isCtrlDown) { // Mac에서는 Cmd, Windows에서는 Ctrl
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

// 선택을 토글하는 함수
void CanvasPanel::ToggleSelection(CanvasObject* obj) {
    auto it_selected = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), obj);
    if (it_selected != m_selectedObjects.end()) {
        // 이미 선택된 객체면 선택 해제
        size_t index = std::distance(m_selectedObjects.begin(), it_selected); 
        m_selectedObjects.erase(it_selected);
        m_originalPositions.erase(m_originalPositions.begin() + index);
    } else {
        // 선택되지 않은 객체면 선택 추가
        m_selectedObjects.push_back(obj);
        m_originalPositions.push_back(obj->GetPosition());
    }
}

// 마우스가 움직일 때 호출되는 함수 (이동 또는 크기 조정)
void CanvasPanel::OnMouseMove(wxMouseEvent& event) {
    wxPoint currentPos = event.GetPosition();

    // 사용자가 빈 공간을 클릭한 후, 드래그하여 여러 객체를 선택할 수 있는 영역을 지정하고 있는 상태
    if (m_isSelecting) {
        m_selectionEndPos = currentPos;
        Refresh();
        return;
    }

    // 객체들이 이미 선택이 되어서 드래그 하고 있는 경우
    if (m_isDragging && !m_isResizing && !m_selectedObjects.empty()) {
        HandleDragging(currentPos);
        Refresh();
        return;
    }

    // 리사이징 중일 때
    if (m_isResizing && !m_selectedObjects.empty()) {
        HandleResizing(currentPos);
        Refresh();
        return;
    }

    // 드래그도 리사이징도 아닌 경우, 커서 상태 업데이트
    UpdateCursorState(currentPos);
}

// 드래깅 처리 함수
void CanvasPanel::HandleDragging(const wxPoint& currentPos) {
    wxPoint delta = currentPos - m_dragStartPos;

    for (size_t i = 0; i < m_selectedObjects.size(); ++i) {
        wxPoint newPosition = m_originalPositions[i] + delta; 
        m_selectedObjects[i]->SetPosition(newPosition);
    }

    // 선택된 객체가 단일 객체인 경우 PropertyPanel 업데이트
    if (m_selectedObjects.size() == 1 && m_propertyPanel) {
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    }
}

// 리사이징 처리 함수
void CanvasPanel::HandleResizing(const wxPoint& currentPos) {
    for (auto& obj : m_selectedObjects) {
        LineObject* lineObj = dynamic_cast<LineObject*>(obj);
        if (lineObj) {
            if (m_resizingStartPoint) {
                lineObj->SetStartPoint(currentPos);
            } else {
                lineObj->SetEndPoint(currentPos);
            }
        } else {
            ResizeOtherObject(obj, currentPos);
        }
    }

    // 선택된 객체가 단일 객체인 경우 PropertyPanel 업데이트
    if (m_selectedObjects.size() == 1 && m_propertyPanel) {
        m_propertyPanel->SetSelectedObjects(m_selectedObjects);
    }
}

// 다른 객체의 리사이징 처리 함수
void CanvasPanel::ResizeOtherObject(CanvasObject* obj, const wxPoint& currentPos) {
    wxPoint delta = currentPos - m_resizeStartPos;
    wxSize newSize = m_originalSize;
    newSize.SetWidth(newSize.GetWidth() + delta.x);
    newSize.SetHeight(newSize.GetHeight() + delta.y);

    // 최소 크기 제한
    newSize.SetWidth(std::max(newSize.GetWidth(), 10));
    newSize.SetHeight(std::max(newSize.GetHeight(), 10));

    obj->SetSize(newSize);
}

// 커서 상태 업데이트 함수
void CanvasPanel::UpdateCursorState(const wxPoint& currentPos) {
    bool onEdge = std::any_of(m_objects.rbegin(), m_objects.rend(),
        [&](CanvasObject* obj) -> bool {
            return obj->isMouseNearEdge(currentPos, 10);
        });

    SetCursor(onEdge ? wxCURSOR_CROSS : wxCURSOR_ARROW);
}


void CanvasPanel::OnMouseClickEnd(wxMouseEvent& event) {
    m_isMouseClicked = false;
    m_isDragging = false;
    m_isResizing = false;

    if (!m_isSelecting) return; // 
    m_isSelecting = false;

    // 선택 영역 정의
    wxRect selectionRect = CalculateSelectionRect();

    m_selectedObjects.clear();
    m_originalPositions.clear();

    // 선택 영역 내에 있는 모든 객체 선택
    for (auto& obj : m_objects) {
        wxRect objRect(obj->GetPosition(), obj->GetSize());
        if (!selectionRect.Intersects(objRect)) continue;

        // 중복 선택 방지 후 객체 추가
        if (std::find(m_selectedObjects.begin(), m_selectedObjects.end(), obj) == m_selectedObjects.end()) {
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


// 화면 갱신
void CanvasPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this); 

    // 존재하는 모든 객체를 그린다.
    for (CanvasObject* object : m_objects) {
        object->Draw(dc);  
    }

    // 선택된 객체에 테두리 그리기
    for (CanvasObject* obj : m_selectedObjects) {
        LineObject* lineObj = dynamic_cast<LineObject*>(obj);
        if (lineObj) {
            // 선 객체의 경우 시작점과 끝점에 작은 사각형 그리기
            dc.SetPen(wxPen(wxColour(0, 120, 215), 1));
            dc.SetBrush(wxBrush(wxColour(0, 120, 215)));

            wxRect startHandle(lineObj->GetPosition() - wxPoint(5, 5), wxSize(10, 10));
            wxRect endHandle(lineObj->GetEndPoint() - wxPoint(5, 5), wxSize(10, 10));

            dc.DrawRectangle(startHandle);
            dc.DrawRectangle(endHandle);
        } else {
            // 다른 객체에 대한 테두리 그리기
            wxRect rect(obj->GetPosition(), obj->GetSize());
            wxRect paddingRect = rect;
            paddingRect.Inflate(5, 5); // 간격을 두기 위해 인플레이트

            dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_SOLID));
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(paddingRect);
        }
    }

    // 선택 영역 드래그 중인 경우 선택 영역 그리기
    if (m_isSelecting) {
        wxRect selectionRect = CalculateSelectionRect();
        dc.SetPen(wxPen(wxColour(0, 120, 215), 1, wxPENSTYLE_SHORT_DASH));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(selectionRect);
    }
}

// 텍스트 객체 더블 클릭 시 텍스트 수정
void CanvasPanel::OnLeftDClick(wxMouseEvent& event) {
    wxPoint pos = event.GetPosition();

    // 마우스 위치에 있는 텍스트 객체를 찾음
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
        
        // 마우스가 객체 내부에 없으면 다음 객체로
        if (!(*it)->isMouseInside(pos)) {
            continue; 
        }

        // 텍스트 객체가 아니면 다음 객체로
        auto* textObject = dynamic_cast<TextObject*>(*it);
        if (!textObject) {
            continue;
        }

        wxTextEntryDialog dialog(this, "텍스트를 입력하세요:", "텍스트 수정", textObject->GetText());

        // 사용자 입력값으로 텍스트 업데이트
        if (dialog.ShowModal() == wxID_OK) {
            textObject->SetText(dialog.GetValue().ToStdString());
            Refresh();
        }
        return; 
    }
}

wxRect CanvasPanel::CalculateSelectionRect() const {
    int left = std::min(m_selectionStartPos.x, m_selectionEndPos.x);
    int top = std::min(m_selectionStartPos.y, m_selectionEndPos.y);
    int right = std::max(m_selectionStartPos.x, m_selectionEndPos.x);
    int bottom = std::max(m_selectionStartPos.y, m_selectionEndPos.y);

    return wxRect(left, top, right - left, bottom - top);
}