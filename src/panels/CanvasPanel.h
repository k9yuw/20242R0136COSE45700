#ifndef CANVASPANEL_H
#define CANVASPANEL_H

#include <wx/wx.h>
#include <vector>
#include "objects/CanvasObject.h"
#include "objects/TextObject.h"

class PropertyPanel;

class CanvasPanel : public wxPanel {
public:
    // 생성자: 부모 윈도우를 받아서 초기화합
    CanvasPanel(wxWindow* parentWindow);
    ~CanvasPanel();

    // 객체를 추가하는 함수
    void AddObject(CanvasObject* object);

    // 선택된 객체들을 반환하는 함수
    const std::vector<CanvasObject*>& GetSelectedObjects() const;

    // 객체 리스트를 반환하는 함수
    const std::vector<CanvasObject*>& GetObjects() const;

    // 선택된 객체들을 설정하는 함수
    void SetSelectedObjects(const std::vector<CanvasObject*>& objects);

    // 화면을 갱신하는 함수
    void RefreshCanvas();

    // PropertyPanel 설정 함수
    void SetPropertyPanel(PropertyPanel* propertyPanel);

    // z-order 재정렬 함수
    void ReorderObjectsByZOrder();

    bool m_resizingStartPoint; // true이면 시작점을 리사이징, false이면 끝점을 리사이징

private:
    // 이벤트 핸들러 함수들 선언
    void OnPaint(wxPaintEvent& event);
    void OnMouseClickStart(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event); 
    void OnMouseClickEnd(wxMouseEvent& event);
    void OnEditText(wxMouseEvent& event);
    void OnLeftDClick(wxMouseEvent& event);

    bool m_isMouseClicked;
    bool m_isDragging;
    bool m_isResizing;

    // 선택 영역 드래그 관련 변수
    bool m_isSelecting;
    wxPoint m_selectionStartPos;
    wxPoint m_selectionEndPos;

    // 객체 관리 벡터
    std::vector<CanvasObject*> m_objects; // 전체 객체들을 관리하는 벡터
    std::vector<CanvasObject*> m_selectedObjects; // 선택된 객체를 가리키는 벡터
    std::vector<wxPoint> m_originalPositions; // 선택된 객체들의 원래 위치

    // 리사이징 관련 변수
    wxPoint m_resizeStartPos; // 리사이징 시작 시 마우스 위치
    wxPoint m_mouseLastPosition;
    wxPoint m_dragStartPos;
    wxPoint m_originalPosition;
    wxSize m_originalSize;

    PropertyPanel* m_propertyPanel;

    wxDECLARE_EVENT_TABLE();
};

#endif
