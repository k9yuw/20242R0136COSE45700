#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "CanvasObject.h"
#include <string>

class TextObject : public CanvasObject {
public:
    TextObject(const wxPoint& position, const wxSize& size, const std::string& text, int fontSize = 12);

    void Draw(wxDC& dc) override;

    void SetText(const std::string& newText);
    const std::string& GetText() const;

    void SetFontSize(int newSize);
    int GetFontSize() const;

private:
    std::string m_text;
    int fontSize;
};

#endif // TEXTOBJECT_H
