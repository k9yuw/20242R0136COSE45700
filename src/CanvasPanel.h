#ifndef CANVASPANEL_H
#define CANVASPANEL_H

#include <wx/wx.h>
#include <vector>
#include "CanvasObject.h"

// CanvasPanel 클래스 선언
class CanvasPanel : public wxPanel {
public:
    // 생성자: 부모 윈도우를 받아서 초기화합
    CanvasPanel(wxWindow* parentWindow);

    // 객체를 추가하는 함수
    void AddObject(CanvasObject* object);

    // 선택된 객체를 반환하는 함수
    CanvasObject* GetSelectedObject() const;

    // 객체 리스트를 반환하는 함수
    const std::vector<CanvasObject*>& GetObjects() const;

    // 선택된 객체를 설정하는 함수
    void SetSelectedObject(CanvasObject* object);

    // 화면을 갱신하는 함수
    void RefreshCanvas();

private:
    // 이벤트 핸들러 함수들 선언
    void OnPaint(wxPaintEvent& event);
    void OnMouseClickStart(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event); 
    void OnMouseClickEnd(wxMouseEvent& event);


    bool m_isMouseClicked;
    bool m_isDragging;
    bool m_isResizing;

    // 객체들을 관리하는 벡터
    std::vector<CanvasObject*> m_objects;

    // 선택된 객체를 가리키는 포인터
    CanvasObject* m_selectedObject;

    wxPoint m_mouseLastPoint;
    wxPoint m_dragStartPos;
    wxPoint m_originalPosition;
    wxSize m_originalSize;

    wxDECLARE_EVENT_TABLE();
};

#endif
