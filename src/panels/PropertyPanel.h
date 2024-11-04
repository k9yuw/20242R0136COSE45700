#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <wx/wx.h>
#include "CanvasPanel.h"
#include <vector>


class CanvasPanel;

class PropertyPanel : public wxPanel {
public:
    PropertyPanel(wxWindow* parent, CanvasPanel* canvasPanel);

    void SetSelectedObjects(const std::vector<CanvasObject*>& objects);

private:

    void OnPositionXChanged(wxCommandEvent& event);
    void OnPositionYChanged(wxCommandEvent& event);
    void OnWidthChanged(wxCommandEvent& event);
    void OnHeightChanged(wxCommandEvent& event);
    void OnZOrderChanged(wxCommandEvent& event);
    void OnFontSizeChanged(wxCommandEvent& event);

    void OnAddImage(wxCommandEvent& event);
    void OnAddText(wxCommandEvent& event);
    void OnAddLine(wxCommandEvent& event);
    void OnAddRectangle(wxCommandEvent& event);
    void OnAddEllipse(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

    CanvasPanel* m_canvasPanel;
    std::vector<CanvasObject*> m_selectedObjects;

    // 위치와 크기 입력 컨트롤들
    wxTextCtrl* m_positionXCtrl;
    wxTextCtrl* m_positionYCtrl;
    wxTextCtrl* m_widthCtrl;
    wxTextCtrl* m_heightCtrl;
    wxTextCtrl* m_zOrderCtrl;
    wxTextCtrl* m_fontSizeCtrl;

    // 이벤트 핸들러 재진입 방지 플래그
    bool m_ignoreEvents;
};

enum {
    ID_POSITION_X = wxID_HIGHEST + 1,
    ID_POSITION_Y,
    ID_WIDTH,
    ID_HEIGHT,
    ID_ZORDER,
    ID_FONT_SIZE,
    ID_ADD_IMAGE,    
    ID_ADD_TEXT,       
    ID_ADD_LINE,        
    ID_ADD_RECTANGLE,  
    ID_ADD_ELLIPSE     
};

#endif 
