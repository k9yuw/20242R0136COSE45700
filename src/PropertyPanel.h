#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <wx/wx.h>
#include "CanvasObject.h"
#include "CanvasPanel.h"

class PropertyPanel : public wxPanel {
public:
    PropertyPanel(wxWindow* parentWindow);

    void SetSelectedObject(CanvasObject* object);
    void SetCanvasPanel(CanvasPanel* canvasPanel);

private:
    void OnPositionChanged(wxCommandEvent& event);
    void OnSizeChanged(wxCommandEvent& event);
    void OnZOrderChanged(wxCommandEvent& event);

    CanvasObject* m_selectedObject;
    CanvasPanel* m_canvasPanel;

    wxTextCtrl* m_positionXCtrl;
    wxTextCtrl* m_positionYCtrl;
    wxTextCtrl* m_widthCtrl;
    wxTextCtrl* m_heightCtrl;
    wxTextCtrl* m_zOrderCtrl;

    wxDECLARE_EVENT_TABLE();
};

#endif
