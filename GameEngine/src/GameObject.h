#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include<iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Component.h"
#include<glm/glm.hpp>
#include<functional>

class GameObject{

    public :
    glm::vec3 position;
    glm::vec3 rotationXYZ;
    glm::vec3 scale;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    std::vector<std::function<void()>> awakeCallbacks;
    std::vector<std::function<void()>> startCallbacks;
    std::vector<std::function<void()>> updateCallbacks;

    GameObject(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 rotationXYZ = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f));

    template <typename T, typename... Args>
    void AddComponent(Args... args)
    {
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        comp->gameObject = this;
        components[typeid(T)] = comp;
    }

    template <typename T>
    T* GetComponent()
    {
        auto it = components.find(typeid(T));
        if (it != components.end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    void awake();
    void start();
    void update();

    // template <typename T>
    // void RemoveComponent();

};

#endif
