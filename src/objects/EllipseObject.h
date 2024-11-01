#pragma once
#include "CanvasObject.h"

class EllipseObject : public CanvasObject {
public:
    EllipseObject(const wxPoint& position, const wxSize& size);
    void Draw(wxDC& dc) override;
};