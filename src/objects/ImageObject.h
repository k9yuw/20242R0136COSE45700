#pragma once

#include "CanvasObject.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <functional>

class ImageObject : public CanvasObject {
public:
    ImageObject(const wxPoint& position, const wxSize& size, const wxBitmap& bitmap);
    virtual ~ImageObject() {}

    virtual void Draw(wxDC& dc) override;
    virtual bool isMouseNearEdge(const wxPoint& pos, int tolerance) const override;
    virtual bool isMouseInside(const wxPoint& pos) const override;

private:
    // 원본 비트맵을 보관
    wxBitmap m_originalBitmap;

    // 스케일된 비트맵을 캐시
    wxBitmap m_scaledBitmap;

    // 스케일이 필요한지 여부를 나타내는 플래그
    bool m_needsRescale;

    // 크기 변경 시 호출되는 콜백 함수
    void OnSizeChanged();
};
