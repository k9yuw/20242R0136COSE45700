#include "App.h"
#include "MainFrame.h"

bool App::OnInit() {

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxBMPHandler);
    
    MainFrame* mainFrame = new MainFrame("COSE457 그림판 프로젝트: FullHouse", wxPoint(50, 50), wxSize(800, 600));
    mainFrame->Show(true);

    return true;
}
