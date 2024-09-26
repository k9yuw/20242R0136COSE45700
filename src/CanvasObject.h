#ifndef CANVASOBJECT_H
#define CANVASOBJECT_H

#include <wx/wx.h>

class CanvasObject {
public:
    CanvasObject(const wxPoint& position, const wxSize& size);
    virtual ~CanvasObject() {}

    // 그리기 함수: virtual이라 파생 클래스에서 반드시 구현해야함
    virtual void Draw(wxDC& dc) = 0;

    // 마우스 좌표가 객체 내부에 있는지 확인
    virtual bool isMouseInside(const wxPoint& point) const;

    // Getter, Setter
    wxPoint GetPosition() const;
    void SetPosition(const wxPoint& position);

    wxSize GetSize() const;
    void SetSize(const wxSize& size);

    int GetZOrder() const;
    void SetZOrder(int zOrder);

protected:
    wxPoint m_position;
    wxSize m_size;
    int m_zOrder;
};

#endif
