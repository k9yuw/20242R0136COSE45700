#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <wx/wx.h>
#include "CanvasObject.h"
#include "CanvasPanel.h"

class PropertyPanel : public wxPanel {
public:
    PropertyPanel(wxWindow* parentWindow);

    void SetSelectedObject(CanvasObject* object);  // 선택된 객체의 속성을 설정하는 함수
    void SetCanvasPanel(CanvasPanel* canvasPanel); // 캔버스 패널을 설정하여 refresh

private:
    void OnPositionXChanged(wxCommandEvent& event); // x좌표 변경 시 호출되는 이벤트 핸들러
    void OnPositionYChanged(wxCommandEvent& event); // y좌표 변경 시 호출되는 이벤트 핸들러
    void OnWidthChanged(wxCommandEvent& event);     // width 변경 시 호출되는 이벤트 핸들러
    void OnHeightChanged(wxCommandEvent& event);    // height 변경 시 호출되는 이벤트 핸들러
    void OnZOrderChanged(wxCommandEvent& event);    // z-order 변경 시 호출되는 이벤트 핸들러
    void OnAddImage(wxCommandEvent& event);
    void OnAddText(wxCommandEvent& event);
    void OnAddLine(wxCommandEvent& event);
    void OnAddRectangle(wxCommandEvent& event);
    void OnAddEllipse(wxCommandEvent& event);

    CanvasObject* m_selectedObject;  // 선택된 object를 저장하는 포인터
    CanvasPanel* m_canvasPanel;      // 캔버스 패널을 저장하는 포인터

    wxTextCtrl* m_positionXCtrl;     // x좌표 입력하는 컨트롤
    wxTextCtrl* m_positionYCtrl;     // y좌표 입력하는 컨트롤
    wxTextCtrl* m_widthCtrl;         // width 입력하는 컨트롤
    wxTextCtrl* m_heightCtrl;        // height 입력하는 컨트롤
    wxTextCtrl* m_zOrderCtrl;        // z-order 입력하는 컨트롤

    wxDECLARE_EVENT_TABLE();         // 이벤트 테이블 선언: 이벤트 핸들러와 ID 매핑
};

#endif
