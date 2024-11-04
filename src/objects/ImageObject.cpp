#include "ImageObject.h"

ImageObject::ImageObject(const wxPoint& position, const wxSize& size, const wxBitmap& bitmap)
    : CanvasObject(position, size), 
      m_originalBitmap(bitmap), 
      m_scaledBitmap(bitmap), 
      m_needsRescale(false) 
{
    // CanvasObject의 SetOnChangeCallback을 사용하여 크기 변경 시 m_needsRescale을 true로 설정
    SetOnChangeCallback(std::bind(&ImageObject::OnSizeChanged, this));
}

void ImageObject::OnSizeChanged() {
    // 크기가 변경되었으므로 다음 그리기 때 스케일을 수행
    m_needsRescale = true;
}

void ImageObject::Draw(wxDC& dc) {
    if (m_needsRescale) {
        // 원본 비트맵을 현재 크기에 맞게 스케일링
        wxImage image = m_originalBitmap.ConvertToImage();
        wxImage scaledImage = image.Scale(m_size.GetWidth(), m_size.GetHeight(), wxIMAGE_QUALITY_HIGH);

        // 스케일된 이미지를 다시 비트맵으로 변환
        m_scaledBitmap = wxBitmap(scaledImage);

        // 스케일이 완료되었으므로 플래그를 리셋
        m_needsRescale = false;
    }

    // 스케일된 비트맵을 현재 위치에 그리기
    dc.DrawBitmap(m_scaledBitmap, m_position.x, m_position.y, true);
}

bool ImageObject::isMouseNearEdge(const wxPoint& pos, int tolerance) const {
    // 객체의 경계 근처에 마우스가 있는지 확인
    wxRect rect(m_position, m_size);
    return CanvasObject::isMouseNearEdge(pos, tolerance);
}

bool ImageObject::isMouseInside(const wxPoint& pos) const {
    // 객체 내부에 마우스가 있는지 확인
    wxRect rect(m_position, m_size);
    return rect.Contains(pos);
}
