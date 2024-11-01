#pragma once
#include "CanvasObject.h"

class RectangleObject : public CanvasObject {
public:
    RectangleObject(const wxPoint& position, const wxSize& size);
    void Draw(wxDC& dc) override;
};