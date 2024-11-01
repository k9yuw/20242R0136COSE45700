#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CanvasObject.h"
#include <string>

class TextObject : public CanvasObject {
public:
    TextObject(const wxPoint& position, const std::string& text, int fontSize);

    void Draw(wxDC& dc);

    void SetText(const std::string& newText);
    const std::string& GetText() const;

    void SetFontSize(int newSize);
    int GetFontSize() const;

private:
    std::string text;
    int fontSize;
};

#endif 