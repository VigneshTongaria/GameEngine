#include "HierarchyPanel.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../core/Scene.h"
#include "../core/GameObject.h"

HierarchyPanel::HierarchyPanel(Scene* scenePtr) : EditorPanel("Hierarchy"), scene(scenePtr)
{
    
}

void HierarchyPanel::OnImGuiRender()
{
    if (!isVisible || !scene)
        return;

    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
    ImGui::Begin(name.c_str(), &isVisible);

    const auto &gameObjects = scene->gameObjects;
    for (auto &go : gameObjects)
    {
        // if (go->parent == nullptr)
        //     DrawGameObjectNode(*go);
        DrawGameObjectNode(*go);
    }

    ImGui::End();
}

void HierarchyPanel::DrawGameObjectNode(const GameObject &go)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (go.childGameObjects.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (&go == selectedGameObject)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx(go.name.c_str(), flags);

    if (ImGui::IsItemClicked())
        selectedGameObject = const_cast<GameObject*>(&go);

    if (open)
    {
        for (auto *child : go.childGameObjects)
            DrawGameObjectNode(*child);
        ImGui::TreePop();
    }
}
GameObject* HierarchyPanel::GetSelectedGameObject() const
{
     return selectedGameObject;
}
