#include "PropertyPanel.h"
#include "objects/ImageObject.h" 
#include "objects/TextObject.h" 
#include "objects/LineObject.h" 
#include "objects/RectangleObject.h"
#include "objects/EllipseObject.h" 

// 컨트롤 ID 정의
enum {
    ID_POSITION_X = wxID_HIGHEST + 1,
    ID_POSITION_Y,
    ID_WIDTH,
    ID_HEIGHT,
    ID_ZORDER,
    ID_ADD_IMAGE,    
    ID_ADD_TEXT,       
    ID_ADD_LINE,        
    ID_ADD_RECTANGLE,  
    ID_ADD_ELLIPSE     
};

// 이벤트 테이블 설정
wxBEGIN_EVENT_TABLE(PropertyPanel, wxPanel)
    EVT_TEXT(ID_POSITION_X, PropertyPanel::OnPositionXChanged)
    EVT_TEXT(ID_POSITION_Y, PropertyPanel::OnPositionYChanged)
    EVT_TEXT(ID_WIDTH, PropertyPanel::OnWidthChanged)
    EVT_TEXT(ID_HEIGHT, PropertyPanel::OnHeightChanged)
    EVT_TEXT(ID_ZORDER, PropertyPanel::OnZOrderChanged)

    EVT_BUTTON(ID_ADD_IMAGE, PropertyPanel::OnAddImage)
    EVT_BUTTON(ID_ADD_TEXT, PropertyPanel::OnAddText)
    EVT_BUTTON(ID_ADD_LINE, PropertyPanel::OnAddLine)
    EVT_BUTTON(ID_ADD_RECTANGLE, PropertyPanel::OnAddRectangle)
    EVT_BUTTON(ID_ADD_ELLIPSE, PropertyPanel::OnAddEllipse)
wxEND_EVENT_TABLE()

// 생성자
PropertyPanel::PropertyPanel(wxWindow* parent, CanvasPanel* canvasPanel)
    : wxPanel(parent), m_selectedObject(nullptr), m_canvasPanel(canvasPanel) {
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL); // 메인 수직 sizer

    // wxFlexGridSizer로 그리드 형식 구성
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 5, 5); // 2열, 5px 간격으로 배치
    
    // 첫 번째 열은 기본 너비로, 두 번째 열은 확장 가능하도록 설정
    gridSizer->AddGrowableCol(1); 

    // 위치 설정 라벨, 컨트롤 (라벨의 최대 너비 설정)
    wxStaticText* posXLabel = new wxStaticText(this, wxID_ANY, "x값:");
    m_positionXCtrl = new wxTextCtrl(this, ID_POSITION_X, "", wxDefaultPosition, wxSize(80, -1));

    wxStaticText* posYLabel = new wxStaticText(this, wxID_ANY, "y값:");
    m_positionYCtrl = new wxTextCtrl(this, ID_POSITION_Y, "", wxDefaultPosition, wxSize(80, -1));

    // 크기 설정 라벨, 컨트롤 (라벨의 최대 너비 설정)
    wxStaticText* widthLabel = new wxStaticText(this, wxID_ANY, "너비:");
    m_widthCtrl = new wxTextCtrl(this, ID_WIDTH, "", wxDefaultPosition, wxSize(80, -1));

    wxStaticText* heightLabel = new wxStaticText(this, wxID_ANY, "높이:");
    m_heightCtrl = new wxTextCtrl(this, ID_HEIGHT, "", wxDefaultPosition, wxSize(80, -1));

    wxStaticText* zOrderLabel = new wxStaticText(this, wxID_ANY, "z값:");
    m_zOrderCtrl = new wxTextCtrl(this, ID_ZORDER, "", wxDefaultPosition, wxSize(80, -1));

    // GridSizer에 라벨과 컨트롤을 추가
    gridSizer->Add(posXLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_positionXCtrl, 0, wxALL | wxEXPAND, 5);

    gridSizer->Add(posYLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_positionYCtrl, 0, wxALL | wxEXPAND, 5);

    gridSizer->Add(widthLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_widthCtrl, 0, wxALL | wxEXPAND, 5);

    gridSizer->Add(heightLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_heightCtrl, 0, wxALL | wxEXPAND, 5);

    gridSizer->Add(zOrderLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_zOrderCtrl, 0, wxALL | wxEXPAND, 5);

    // 객체 추가 버튼 생성
    wxButton* addImageButton = new wxButton(this, ID_ADD_IMAGE, "이미지 추가");
    wxButton* addTextButton = new wxButton(this, ID_ADD_TEXT, "텍스트 추가");
    wxButton* addLineButton = new wxButton(this, ID_ADD_LINE, "선 추가");
    wxButton* addRectangleButton = new wxButton(this, ID_ADD_RECTANGLE, "사각형 추가");
    wxButton* addEllipseButton = new wxButton(this, ID_ADD_ELLIPSE, "원 추가");

    // 메인 sizer에 gridSizer와 객체 추가 버튼들을 추가
    mainSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(addImageButton, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(addTextButton, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(addLineButton, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(addRectangleButton, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(addEllipseButton, 0, wxALL | wxEXPAND, 5);

    // sizer 설정
    SetSizer(mainSizer);

    // 초기에는 컨트롤들을 비활성화
    m_positionXCtrl->Enable();
    m_positionYCtrl->Enable();
    m_widthCtrl->Disable();
    m_heightCtrl->Disable();
    m_zOrderCtrl->Disable();
}


void PropertyPanel::OnPositionXChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long x;
        if (m_positionXCtrl->GetValue().ToLong(&x)) {  // X 좌표 값 컨트롤에서 읽기
            wxPoint pos = m_selectedObject->GetPosition();
            pos.x = x;                          // 새 X 좌표 설정
            m_selectedObject->SetPosition(pos); // CanvasObject에 위치 반영
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();  // 캔버스를 다시 그려서 변경 사항 반영
            }
        }
    }
}

void PropertyPanel::OnPositionYChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long y;
        if (m_positionYCtrl->GetValue().ToLong(&y)) {   // Y 좌표 값 컨트롤에서 읽기
            wxPoint pos = m_selectedObject->GetPosition();
            pos.y = y;                                 // 새 Y 좌표 설정
            m_selectedObject->SetPosition(pos);        // CanvasObject에 위치 반영
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();         // 캔버스 리프레시
            }
        }
    }
}

void PropertyPanel::OnWidthChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long w;
        if (m_widthCtrl->GetValue().ToLong(&w)) {
            wxSize size = m_selectedObject->GetSize();
            size.SetWidth(w);
            m_selectedObject->SetSize(size);
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();
            }
        }
    }
}

void PropertyPanel::OnHeightChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long h;
        if (m_heightCtrl->GetValue().ToLong(&h)) {
            wxSize size = m_selectedObject->GetSize();
            size.SetHeight(h);
            m_selectedObject->SetSize(size);
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();
            }
        }
    }
}

void PropertyPanel::OnZOrderChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long zOrder;
        if (m_zOrderCtrl->GetValue().ToLong(&zOrder)) {
            m_selectedObject->SetZOrder(static_cast<int>(zOrder));
            if (m_canvasPanel) {
                // Z-순서 변경에 따른 객체 리스트 정렬 함수 호출 필요
                m_canvasPanel->RefreshCanvas();
            }
        }
    }
}

// 이미지 추가 버튼 클릭 시 호출
void PropertyPanel::OnAddImage(wxCommandEvent& event) {
    if (m_canvasPanel) {
        // 이미지 파일 선택 대화상자 열기
        wxFileDialog openFileDialog(this, _("이미지 파일 선택"), "", "",
            "이미지 파일 (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
            return;

        // 이미지 로드
        wxImage image;
        if (!image.LoadFile(openFileDialog.GetPath()))
            return;

        wxBitmap bitmap(image);

        // 이미지 객체 생성
        ImageObject* imageObject = new ImageObject(wxPoint(50, 50), bitmap.GetSize(), bitmap);

        // 캔버스에 추가
        m_canvasPanel->AddObject(imageObject);
        m_canvasPanel->RefreshCanvas();
    }
}


// 텍스트 추가 버튼 클릭 시 호출
void PropertyPanel::OnAddText(wxCommandEvent& event) {
    if (m_canvasPanel) {
        // 임의의 위치와 크기 설정
        wxPoint position(100, 100); // 기본 위치 (임의로 100, 100으로 설정)
        int fontSize = 20;           // 기본 글꼴 크기

        // 텍스트 객체 생성
        TextObject* text = new TextObject(position, "텍스트", fontSize);

        // 캔버스에 텍스트 객체 추가
        m_canvasPanel->AddObject(text);
        m_canvasPanel->RefreshCanvas();
    }
}


// 선 추가 버튼 클릭 시 호출
void PropertyPanel::OnAddLine(wxCommandEvent& event) {
    if (m_canvasPanel) {
        // 선 객체 생성
        LineObject* line = new LineObject(wxPoint(50, 50), wxPoint(150, 150));

        // 캔버스에 추가
        m_canvasPanel->AddObject(line);
        m_canvasPanel->RefreshCanvas();
    }
}

// 사각형 추가 버튼 클릭 시 호출
void PropertyPanel::OnAddRectangle(wxCommandEvent& event) { 
    if (m_canvasPanel) {
        // 사각형 객체 생성
        RectangleObject* rect = new RectangleObject(wxPoint(100, 100), wxSize(100, 50));

        // 캔버스에 추가
        m_canvasPanel->AddObject(rect);
        m_canvasPanel->RefreshCanvas();
    }
}

// 타원 추가 버튼 클릭 시 호출
void PropertyPanel::OnAddEllipse(wxCommandEvent& event) {
    if (m_canvasPanel) {
        // 타원 객체 생성
        EllipseObject* ellipse = new EllipseObject(wxPoint(200, 200), wxSize(80, 80));

        // 캔버스에 추가
        m_canvasPanel->AddObject(ellipse);
        m_canvasPanel->RefreshCanvas();
    }
}
