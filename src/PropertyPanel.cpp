#include "PropertyPanel.h"

// 컨트롤 ID 정의
enum {
    ID_POSITION_X = wxID_HIGHEST + 1, // wxID 중 사용하지 않은 것부터 사용하기 위해 가장 큰 것 + 1 부터 시작
    ID_POSITION_Y,
    ID_WIDTH,
    ID_HEIGHT,
    ID_ZORDER
};

// 이벤트 테이블 설정
wxBEGIN_EVENT_TABLE(PropertyPanel, wxPanel)
    EVT_TEXT(ID_POSITION_X, PropertyPanel::OnPositionXChanged) // ID_POSITION_X 변경 시 OnPositionXChanged 호출
    EVT_TEXT(ID_POSITION_Y, PropertyPanel::OnPositionYChanged)
    EVT_TEXT(ID_WIDTH, PropertyPanel::OnWidthChanged)
    EVT_TEXT(ID_HEIGHT, PropertyPanel::OnHeightChanged)
    EVT_TEXT(ID_ZORDER, PropertyPanel::OnZOrderChanged)
wxEND_EVENT_TABLE()

// 생성자
PropertyPanel::PropertyPanel(wxWindow* parent)
    : wxPanel(parent), m_selectedObject(nullptr), m_canvasPanel(nullptr) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); // 수직으로 컨트롤 배치하는 박스인 sizer 생성

    // 위치 설정 라벨, 컨트롤
    wxStaticText* posLabel = new wxStaticText(this, wxID_ANY, "Position:");
    m_positionXCtrl = new wxTextCtrl(this, ID_POSITION_X);
    m_positionYCtrl = new wxTextCtrl(this, ID_POSITION_Y);

    // 크기 설정 라벨, 컨트롤
    wxStaticText* sizeLabel = new wxStaticText(this, wxID_ANY, "Size:");
    m_widthCtrl = new wxTextCtrl(this, ID_WIDTH);
    m_heightCtrl = new wxTextCtrl(this, ID_HEIGHT);

    // Z-순서 설정 라벨, 컨트롤
    wxStaticText* zOrderLabel = new wxStaticText(this, wxID_ANY, "Z-Order:");
    m_zOrderCtrl = new wxTextCtrl(this, ID_ZORDER);

    // sizer에 각 라벨, 컨트롤 추가
    sizer->Add(posLabel, 0, wxALL, 5);
    sizer->Add(m_positionXCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(m_positionYCtrl, 0, wxALL | wxEXPAND, 5);

    sizer->Add(sizeLabel, 0, wxALL, 5);
    sizer->Add(m_widthCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(m_heightCtrl, 0, wxALL | wxEXPAND, 5);

    sizer->Add(zOrderLabel, 0, wxALL, 5);
    sizer->Add(m_zOrderCtrl, 0, wxALL | wxEXPAND, 5);

    SetSizer(sizer);

    // 초기에는 컨트롤들을 비활성화
    m_positionXCtrl->Disable();
    m_positionYCtrl->Disable();
    m_widthCtrl->Disable();
    m_heightCtrl->Disable();
    m_zOrderCtrl->Disable();
}

// 값 변경시 canvas panel refresh 하기 위함. 
void PropertyPanel::SetCanvasPanel(CanvasPanel* canvasPanel) {
    m_canvasPanel = canvasPanel;
}

// object가 선택되면 -> 선택된 object의 속성값을 property panel에 띄워주는 함수
void PropertyPanel::SetSelectedObject(CanvasObject* object) {
    m_selectedObject = object; 

    if (object) {
        // object 속성 값을 컨트롤에 표시
        m_positionXCtrl->SetValue(wxString::Format("%d", object->GetPosition().x));
        m_positionYCtrl->SetValue(wxString::Format("%d", object->GetPosition().y));
        m_widthCtrl->SetValue(wxString::Format("%d", object->GetSize().GetWidth()));
        m_heightCtrl->SetValue(wxString::Format("%d", object->GetSize().GetHeight()));
        m_zOrderCtrl->SetValue(wxString::Format("%d", object->GetZOrder()));

        // 컨트롤 활성화
        m_positionXCtrl->Enable();
        m_positionYCtrl->Enable();
        m_widthCtrl->Enable();
        m_heightCtrl->Enable();
        m_zOrderCtrl->Enable();
    } else {
        // 선택된 object가 없으면 아무것도 보여주지 않는다: 컨트롤을 비우고 비활성화
        m_positionXCtrl->SetValue("");
        m_positionYCtrl->SetValue("");
        m_widthCtrl->SetValue("");
        m_heightCtrl->SetValue("");
        m_zOrderCtrl->SetValue("");

        m_positionXCtrl->Disable();
        m_positionYCtrl->Disable();
        m_widthCtrl->Disable();
        m_heightCtrl->Disable();
        m_zOrderCtrl->Disable();
    }
}

void PropertyPanel::OnPositionXChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long x;
        if (m_positionXCtrl->GetValue().ToLong(&x)) {
            wxPoint pos = m_selectedObject->GetPosition();
            pos.x = x;
            m_selectedObject->SetPosition(pos);
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();
            }
        }
    }
}

void PropertyPanel::OnPositionYChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long y;
        if (m_positionYCtrl->GetValue().ToLong(&y)) {
            wxPoint pos = m_selectedObject->GetPosition();
            pos.y = y;
            m_selectedObject->SetPosition(pos);
            if (m_canvasPanel) {
                m_canvasPanel->RefreshCanvas();
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
