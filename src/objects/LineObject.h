#pragma once
#include "CanvasObject.h"

class LineObject : public CanvasObject {
public:
    LineObject(const wxPoint& start, const wxPoint& end);
    void Draw(wxDC& dc) override;
    bool isMouseInside(const wxPoint& point) const override;
    bool isMouseNearEdge(const wxPoint& point, int margin) const override;
    void SetPosition(const wxPoint& point) override; // const 제거
    const wxPoint& GetEndPoint() const;
    void SetStartPoint(const wxPoint& start);
    void SetEndPoint(const wxPoint& end);

private:
    wxPoint m_endPoint;
};
