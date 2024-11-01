#include "MainFrame.h"
#include "panels/CanvasPanel.h"
#include "panels/PropertyPanel.h"
#include <wx/menu.h>

// 이벤트 테이블 설정
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()

// 생성자
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    // 로그 메시지를 콘솔로 출력하도록 설정
    wxLog::SetActiveTarget(new wxLogStderr());

    CreateMenuBar();
    InitStatusBar();
    SetupPanels(); 
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
    SetStatusText("Welcome to the Canvas Application!");
}

void MainFrame::SetupPanels() {
    // CanvasPanel과 PropertyPanel 생성
    m_canvasPanel = new CanvasPanel(this);
    m_propertyPanel = new PropertyPanel(this, m_canvasPanel);

    // CanvasPanel에 PropertyPanel 설정
    m_canvasPanel->SetPropertyPanel(m_propertyPanel);
    wxLogMessage("PropertyPanel has been set in CanvasPanel.");

    // 수평 방향으로 두 패널을 배치하기 위한 Sizer 생성
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Property 패널을 고정 크기로 추가 
    mainSizer->Add(m_propertyPanel, 0, wxALIGN_TOP | wxALL, 5); 

    // Canvas 패널은 남은 공간을 차지하도록 추가
    mainSizer->Add(m_canvasPanel, 1, wxEXPAND | wxALL, 5);  

    // Sizer를 프레임에 설정하고 레이아웃 적용
    SetSizer(mainSizer);
    Layout();

    // 프레임의 최소 크기 설정
    SetMinSize(wxSize(600, 400));
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
