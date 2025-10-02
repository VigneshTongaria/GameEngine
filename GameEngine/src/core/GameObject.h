#pragma once
#include<iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include<glm/glm.hpp>
#include<functional>

class Component;
class Scene;

struct Transform
{
    public :
    glm::vec3 position; 
    glm::vec3 rotationXYZ; 
    glm::vec3 scale;
};


class GameObject{

    public :
    Transform transform;
    Scene* scene;
    GameObject* parent;
    std::vector<GameObject*> childGameObjects;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    std::vector<std::function<void()>> awakeCallbacks;
    std::vector<std::function<void()>> startCallbacks;
    std::vector<std::function<void()>> updateCallbacks;

    GameObject(Scene* scene, GameObject* parent,
        glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), 
        glm::vec3 rotationXYZ = glm::vec3(0.0f,0.0f,0.0f), 
        glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f));
    GameObject(Scene* scene, GameObject* parent,Transform transform);

    template <typename T, typename... Args>
    void AddComponent(Args&&... args);

    template <typename T>
    T* GetComponent();
    void awake();
    void start();
    void update();

    // template <typename T>
    // void RemoveComponent();

};