// PropertyPanel.cpp
#include "PropertyPanel.h"
#include "CanvasPanel.h"
#include "objects/CanvasObject.h"
#include "objects/TextObject.h" 
#include "objects/ImageObject.h"
#include "objects/LineObject.h"
#include "objects/RectangleObject.h"
#include "objects/EllipseObject.h"

#include <algorithm>

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
    : wxPanel(parent), m_canvasPanel(canvasPanel), m_ignoreEvents(false) {
    
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
    m_positionXCtrl->Disable();
    m_positionYCtrl->Disable();
    m_widthCtrl->Disable();
    m_heightCtrl->Disable();
    m_zOrderCtrl->Disable();
}

// 선택된 객체들을 설정하는 함수
void PropertyPanel::SetSelectedObjects(const std::vector<CanvasObject*>& objects) {
    m_ignoreEvents = true;  // 이벤트 핸들러 재진입 방지
    m_selectedObjects = objects;

    if (m_selectedObjects.empty()) {
        // 선택된 객체가 없으면 모든 컨트롤 비활성화
        m_positionXCtrl->Disable();
        m_positionYCtrl->Disable();
        m_widthCtrl->Disable();
        m_heightCtrl->Disable();
        m_zOrderCtrl->Disable();

        // 컨트롤 내용 초기화
        m_positionXCtrl->SetValue("");
        m_positionYCtrl->SetValue("");
        m_widthCtrl->SetValue("");
        m_heightCtrl->SetValue("");
        m_zOrderCtrl->SetValue("");

        m_ignoreEvents = false;
        Layout();  
        Refresh();  
        return;
    }

    // 컨트롤 활성화
    m_positionXCtrl->Enable();
    m_positionYCtrl->Enable();
    m_widthCtrl->Enable();
    m_heightCtrl->Enable();
    m_zOrderCtrl->Enable();

    if (m_selectedObjects.size() == 1) {
        // 객체가 하나만 선택된 경우 해당 객체의 속성을 표시
        CanvasObject* obj = m_selectedObjects[0];

        wxPoint pos = obj->GetPosition();
        wxSize size = obj->GetSize();
        int zOrder = obj->GetZOrder();

        m_positionXCtrl->SetValue(wxString::Format("%d", pos.x));
        m_positionYCtrl->SetValue(wxString::Format("%d", pos.y));
        m_widthCtrl->SetValue(wxString::Format("%d", size.GetWidth()));
        m_heightCtrl->SetValue(wxString::Format("%d", size.GetHeight()));
        m_zOrderCtrl->SetValue(wxString::Format("%d", zOrder));
    } else {
        // 다중 선택된 경우 속성 값을 공백으로 설정
        m_positionXCtrl->SetValue("");
        m_positionYCtrl->SetValue("");
        m_widthCtrl->SetValue("");
        m_heightCtrl->SetValue("");
        m_zOrderCtrl->SetValue("");
    }

    m_ignoreEvents = false;
    Layout();  
    Refresh(); 
}

// 이벤트 핸들러 구현 (모든 선택된 객체에 적용)
void PropertyPanel::OnPositionXChanged(wxCommandEvent& event) {
    if (m_ignoreEvents) return;  // 이벤트 핸들러 재진입 방지

    long x;
    if (m_positionXCtrl->GetValue().ToLong(&x)) {
        for (auto& obj : m_selectedObjects) {
            wxPoint pos = obj->GetPosition();
            pos.x = static_cast<int>(x);
            obj->SetPosition(pos);
        }
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnPositionYChanged(wxCommandEvent& event) {
    if (m_ignoreEvents) return;

    long y;
    if (m_positionYCtrl->GetValue().ToLong(&y)) {
        for (auto& obj : m_selectedObjects) {
            wxPoint pos = obj->GetPosition();
            pos.y = static_cast<int>(y);
            obj->SetPosition(pos);
        }
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnWidthChanged(wxCommandEvent& event) {
    if (m_ignoreEvents) return;

    long w;
    if (m_widthCtrl->GetValue().ToLong(&w)) {
        for (auto& obj : m_selectedObjects) {
            wxSize size = obj->GetSize();
            size.SetWidth(static_cast<int>(w));
            obj->SetSize(size);
        }
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnHeightChanged(wxCommandEvent& event) {
    if (m_ignoreEvents) return;

    long h;
    if (m_heightCtrl->GetValue().ToLong(&h)) {
        for (auto& obj : m_selectedObjects) {
            wxSize size = obj->GetSize();
            size.SetHeight(static_cast<int>(h));
            obj->SetSize(size);
        }
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnZOrderChanged(wxCommandEvent& event) {
    if (m_ignoreEvents) return;

    long zOrder;
    if (m_zOrderCtrl->GetValue().ToLong(&zOrder)) {
        for (auto& obj : m_selectedObjects) {
            obj->SetZOrder(static_cast<int>(zOrder));
        }
        wxLogMessage("PropertyPanel: z값 변경됨. 새 z값: %ld", zOrder);
        if (m_canvasPanel) {
            // z-order 변경 후, 객체 목록 재정렬
            m_canvasPanel->ReorderObjectsByZOrder();
            m_canvasPanel->RefreshCanvas();
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
        // 임의의 위치과 크기 설정
        wxPoint position(100, 100); // 기본 위치
        wxSize size(100, 30);       // 기본 크기

        // 텍스트 객체 생성
        TextObject* text = new TextObject(position, size, "텍스트");

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
