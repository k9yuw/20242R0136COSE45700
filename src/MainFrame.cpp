#include "MainFrame.h"
#include "CanvasPanel.h" 
#include <wx/menu.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    CreateMenuBar();       
    InitStatusBar();    
    SetupCanvasPanel();  
}


void MainFrame::CreateMenuBar() {
    // 메뉴 바 생성
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT, "E&xit\tAlt-X", "Exit application");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);
}


void MainFrame::InitStatusBar() {
    wxFrame::CreateStatusBar();
    SetStatusText("FullHouse");
}


void MainFrame::SetupCanvasPanel() {
    m_canvasPanel = new CanvasPanel(this);

    // 메인 Sizer 생성
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // 상단 여백 추가
    mainSizer->Add(m_canvasPanel, 1, wxEXPAND | wxALL, 20);

    // Sizer를 프레임에 설정하고 레이아웃 적용
    SetSizer(mainSizer);
    Layout();

    // 프레임의 최소 크기 설정
    SetMinSize(wxSize(450, 350));
}


// 종료 메뉴 선택 시 호출되는 이벤트 핸들러
void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
