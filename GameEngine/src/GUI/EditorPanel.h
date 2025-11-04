#pragma once
#include <string>

class EditorPanel {
protected:
    std::string name;
    bool isVisible = true;

public:
    EditorPanel(const std::string& panelName)
        : name(panelName), isVisible(true) {}

    virtual ~EditorPanel() = default;

    virtual void OnImGuiRender() = 0; // abstract GUI draw function

    virtual void Show()  { isVisible = true;  }
    virtual void Hide()  { isVisible = false; }
    virtual bool IsVisible() const { return isVisible; }

    const std::string& GetName() const { return name; }
};