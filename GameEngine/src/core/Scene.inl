#pragma once
#include "Scene.h"
#include "GameObject.inl"

template <typename T>
void Scene::addComponentToScene(std::shared_ptr<T> comp)
{
    COMPONENT_TYPE type = GetComponentTypeEnum<T>();
    componentsMap[type].push_back(comp);
    updateSceneComponentsType(type);
}

template <typename T, typename... Args>
GameObject* Scene::addNewGameObjectToScene(GameObject *parent, Transform transform, Args &&...args)
{
    gameObjects.emplace_back(std::make_unique<GameObject>(this, parent, transform));
    GameObject *_gO = gameObjects.back().get();

    if constexpr (!std::is_same_v<T, void>)
    {
        _gO->AddComponent<T>(std::forward<Args>(args)...);
    }

    return _gO;
}