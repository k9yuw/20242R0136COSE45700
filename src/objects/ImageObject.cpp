#include "ImageObject.h"

ImageObject::ImageObject(const wxPoint& position, const wxSize& size, const wxBitmap& bitmap)
    : CanvasObject(position, size), m_bitmap(bitmap) {
}

void ImageObject::Draw(wxDC& dc) {
    dc.DrawBitmap(m_bitmap, m_position.x, m_position.y, true);
}