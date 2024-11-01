#pragma once
#include "CanvasObject.h"
#include <wx/bitmap.h>

class ImageObject : public CanvasObject {
public:
    ImageObject(const wxPoint& position, const wxSize& size, const wxBitmap& bitmap);
    void Draw(wxDC& dc) override;

private:
    wxBitmap m_bitmap;
};