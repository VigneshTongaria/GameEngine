#pragma once
#include "EditorPanel.h"
#include "Scene.h"
#include "imgui/imgui.h"

class GameObject;
class Scene;

class HierarchyPanel : public EditorPanel {
private:
    Scene* scene = nullptr;
    GameObject* selectedGameObject = nullptr;

public:
    HierarchyPanel(Scene* scenePtr)
        : EditorPanel("Hierarchy"), scene(scenePtr) {}

    void OnImGuiRender() override {
        if (!isVisible || !scene) return;

        ImGui::Begin(name.c_str(), &isVisible);

        const auto& gameObjects = scene->GetGameObjects();
        for (auto& go : gameObjects) {
            if (go->parent == nullptr)
                DrawGameObjectNode(*go);
        }

        ImGui::End();
    }

    void DrawGameObjectNode(GameObject& go) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (go.children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
        if (&go == selectedGameObject) flags |= ImGuiTreeNodeFlags_Selected;

        bool open = ImGui::TreeNodeEx(go.name.c_str(), flags);

        if (ImGui::IsItemClicked())
            selectedGameObject = &go;

        if (open) {
            for (auto* child : go.children)
                DrawGameObjectNode(*child);
            ImGui::TreePop();
        }
    }

    GameObject* GetSelected() const { return selectedGameObject; }
};
