#pragma once
#include "CanvasObject.h"

class LineObject : public CanvasObject {
public:
    LineObject(const wxPoint& start, const wxPoint& end);
    void Draw(wxDC& dc) override;

private:
    wxPoint m_endPoint;
};