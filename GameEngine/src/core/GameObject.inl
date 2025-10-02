#pragma once
#include "GameObject.h"
#include "Scene.h"

template <typename T, typename... Args>
void GameObject::AddComponent(Args &&...args)
{
    auto comp = std::make_shared<T>(std::forward<Args>(args)...);
    comp->gameObject = this;
    components[typeid(T)] = comp;

    if (scene != nullptr)
    {
        scene->addComponentToScene<T>(comp);
    }
}

template <typename T>
T* GameObject::GetComponent()
{
    auto it = components.find(typeid(T));
    if (it != components.end())
    {
        return static_cast<T *>(it->second.get());
    }
    return nullptr;
}