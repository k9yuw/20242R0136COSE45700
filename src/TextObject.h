#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CanvasObject.h"
#include <string>

class TextObject : public CanvasObject {
public:
    TextObject(const wxPoint& position, const std::string& text, int fontSize);

    // Define the member functions without `override` if they are not virtual in `CanvasObject`
    bool IsMouseInside(int mouseX, int mouseY) const;
    bool IsMouseNearEdge(int mouseX, int mouseY) const;

    // Method to draw the text
    void Draw(wxDC& dc);

    // Setters and getters for text and font size
    void SetText(const std::string& newText);
    const std::string& GetText() const;

    void SetFontSize(int newSize);
    int GetFontSize() const;

private:
    std::string text;
    int fontSize;
};

#endif // TEXT_OBJECT_H