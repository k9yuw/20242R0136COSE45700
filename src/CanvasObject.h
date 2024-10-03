#ifndef CANVASOBJECT_H
#define CANVASOBJECT_H

#include <wx/wx.h>
#include <functional> 

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

    // 콜백 등록 함수
    void SetOnChangeCallback(std::function<void()> callback);

protected:
    wxPoint m_position;
    wxSize m_size;
    int m_zOrder;

    // object 상태 변경 시 호출할 콜백
    std::function<void()> m_onChangeCallback;  
};

#endif
