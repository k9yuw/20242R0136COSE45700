#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "CanvasPanel.h" 
#include "PropertyPanel.h"

class MainFrame : public wxFrame {
public:
    // 생성자
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    // 이벤트 핸들러 함수
    void OnExit(wxCommandEvent& event);

    // UI 컴포넌트 설정을 위한 멤버 함수들
    void CreateMenuBar();       // 메뉴 바 생성 
    void InitStatusBar();       // 상태 표시줄 생성
    void SetupPanels();         // 패널 설정 (캔버스 & 속성)
    void SetupCanvasPanel();    // 캔버스 패널 설정
    void SetupPropertyPanel();  // 속성 패널 설정

    // 멤버 변수
    CanvasPanel* m_canvasPanel; 
    PropertyPanel* m_propertyPanel;

    wxDECLARE_EVENT_TABLE(); 
};

#endif
