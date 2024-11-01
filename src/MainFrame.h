#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "panels/CanvasPanel.h"
#include "panels/PropertyPanel.h"

class MainFrame : public wxFrame {
public:
    // 생성자
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    // 이벤트 핸들러 함수
    void OnExit(wxCommandEvent& event);

    void CreateMenuBar();    
    void InitStatusBar();      
    void SetupPanels();       
    void SetupCanvasPanel();    
    void SetupPropertyPanel();  

    // 멤버 변수
    CanvasPanel* m_canvasPanel; 
    PropertyPanel* m_propertyPanel;

    wxDECLARE_EVENT_TABLE(); 
};

#endif
