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
    glm::vec3 position;
    glm::vec3 rotationXYZ;
    glm::vec3 scale;

    Transform() : position(glm::vec3(0.0f)), rotationXYZ(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}

    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : position(pos), rotationXYZ(rot),scale(scl) {}

    mutable bool dirty = true;
    mutable glm::mat4 cachedMatrix{1.0f};

    // Overloaded operators for position
    Transform& operator+=(const glm::vec3& offset) {
        position += offset;
        dirty = true;
        return *this;
    }

    Transform& operator-=(const glm::vec3& offset) {
        position -= offset;
        dirty = true;
        return *this;
    }

    void setScale(const glm::vec3& s) {
        scale = s;
        dirty = true;
    }

    void setRotation(const glm::vec3& r) {
        rotationXYZ = r;
        dirty = true;
    }

    glm::mat4 getTransformationMatrix() const {
        if (dirty) {
            cachedMatrix = glm::mat4(1.0f);
            cachedMatrix = glm::translate(cachedMatrix, position);
            cachedMatrix = glm::rotate(cachedMatrix, glm::radians(rotationXYZ.x), {1,0,0});
            cachedMatrix = glm::rotate(cachedMatrix, glm::radians(rotationXYZ.y), {0,1,0});
            cachedMatrix = glm::rotate(cachedMatrix, glm::radians(rotationXYZ.z), {0,0,1});
            cachedMatrix = glm::scale(cachedMatrix, scale);
            dirty = false;
        }
        return cachedMatrix;
    }
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