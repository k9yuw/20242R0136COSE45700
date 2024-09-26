#include "PropertyPanel.h"

wxBEGIN_EVENT_TABLE(PropertyPanel, wxPanel)
    EVT_TEXT(wxID_ANY, PropertyPanel::OnPositionChanged)
    EVT_TEXT(wxID_ANY, PropertyPanel::OnSizeChanged)
    EVT_TEXT(wxID_ANY, PropertyPanel::OnZOrderChanged)
wxEND_EVENT_TABLE()

PropertyPanel::PropertyPanel(wxWindow* parent)
    : wxPanel(parent), m_selectedObject(nullptr), m_canvasPanel(nullptr) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // 위치 설정 박스
    wxStaticText* posLabel = new wxStaticText(this, wxID_ANY, "Position:");
    m_positionXCtrl = new wxTextCtrl(this, wxID_ANY);
    m_positionYCtrl = new wxTextCtrl(this, wxID_ANY);

    // 크기 설정 박스
    wxStaticText* sizeLabel = new wxStaticText(this, wxID_ANY, "Size:");
    m_widthCtrl = new wxTextCtrl(this, wxID_ANY);
    m_heightCtrl = new wxTextCtrl(this, wxID_ANY);

    // Z-순서 설정 박스
    wxStaticText* zOrderLabel = new wxStaticText(this, wxID_ANY, "Z-Order:");
    m_zOrderCtrl = new wxTextCtrl(this, wxID_ANY);

    sizer->Add(posLabel, 0, wxALL, 5);
    sizer->Add(m_positionXCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(m_positionYCtrl, 0, wxALL | wxEXPAND, 5);

    sizer->Add(sizeLabel, 0, wxALL, 5);
    sizer->Add(m_widthCtrl, 0, wxALL | wxEXPAND, 5);
    sizer->Add(m_heightCtrl, 0, wxALL | wxEXPAND, 5);

    sizer->Add(zOrderLabel, 0, wxALL, 5);
    sizer->Add(m_zOrderCtrl, 0, wxALL | wxEXPAND, 5);

    SetSizer(sizer);
}

// 값 변경시 canvas panel refresh 하기 위함
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
    } else {
        // 선택된 object가 없으면 컨트롤을 비운다. 
        m_positionXCtrl->SetValue("");
        m_positionYCtrl->SetValue("");
        m_widthCtrl->SetValue("");
        m_heightCtrl->SetValue("");
        m_zOrderCtrl->SetValue("");
    }
}

void PropertyPanel::OnPositionChanged(wxCommandEvent& event) {
    if (m_selectedObject) { 
        long x, y;
        m_positionXCtrl->GetValue().ToLong(&x);
        m_positionYCtrl->GetValue().ToLong(&y);
        m_selectedObject->SetPosition(wxPoint(x, y));
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnSizeChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long w, h;
        m_widthCtrl->GetValue().ToLong(&w);
        m_heightCtrl->GetValue().ToLong(&h);
        m_selectedObject->SetSize(wxSize(w, h));
        if (m_canvasPanel) {
            m_canvasPanel->RefreshCanvas();
        }
    }
}

void PropertyPanel::OnZOrderChanged(wxCommandEvent& event) {
    if (m_selectedObject) {
        long zOrder;
        m_zOrderCtrl->GetValue().ToLong(&zOrder);
        m_selectedObject->SetZOrder(zOrder);
        if (m_canvasPanel) {
            // Z-순서 변경에 따른 객체 리스트 정렬 해야됨 -> 함수로 따로 뺄것
            m_canvasPanel->RefreshCanvas();
        }
    }
}
