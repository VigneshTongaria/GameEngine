#pragma once
#include "EditorPanel.h"
#include "../core/Scene.h"
#include "imgui/imgui.h"

class GameObject;
class Scene;

class HierarchyPanel : public EditorPanel {
private:
    Scene* scene = nullptr;
    GameObject* selectedGameObject = nullptr;

public:
    HierarchyPanel(Scene* scenePtr);

    void OnImGuiRender() override;

    void DrawGameObjectNode(const GameObject& go);

    GameObject* GetSelectedGameObject() const;
};
