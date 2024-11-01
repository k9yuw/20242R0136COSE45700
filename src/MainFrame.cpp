#include "MainFrame.h"
#include "panels/CanvasPanel.h"
#include "panels/PropertyPanel.h"
#include <wx/menu.h>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    CreateMenuBar();
    InitStatusBar();
    SetupPanels();  // 캔버스와 속성 패널을 설정하는 함수
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
    // SetStatusText("FullHouse");
}

void MainFrame::SetupPanels() {
    // CanvasPanel과 PropertyPanel 생성
    m_canvasPanel = new CanvasPanel(this);
    m_propertyPanel = new PropertyPanel(this, m_canvasPanel);

    // 수평 방향으로 두 패널을 배치하기 위한 Sizer 생성
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Property 패널을 고정 크기로 추가 (200x400 정도로 설정)
    mainSizer->Add(m_propertyPanel, 0, wxALIGN_TOP | wxALL, 5);  // Property 패널 고정

    // Canvas 패널은 남은 공간을 차지하도록 추가
    mainSizer->Add(m_canvasPanel, 1, wxEXPAND | wxALL, 5);  // Canvas 패널 가변 크기

    // Sizer를 프레임에 설정하고 레이아웃 적용
    SetSizer(mainSizer);
    Layout();

    // 프레임의 최소 크기 설정
    SetMinSize(wxSize(600, 400));
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
