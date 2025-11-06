#pragma once
#include<iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<functional>

class Component;
class Scene;

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotationXYZ;
    glm::vec3 scale;
    Transform* parent;

    bool dirty = false;
    glm::mat4 globalMatrix = glm::mat4(1.0f);
    glm::mat4 localMatrix = glm::mat4(1.0f);

    Transform() : position(glm::vec3(0.0f)), rotationXYZ(glm::vec3(0.0f)), scale(glm::vec3(1.0f)),parent(nullptr) 
    {
        localMatrix = glm::mat4(1.0f);
        localMatrix = glm::translate(localMatrix, position);
        localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.x), { 1,0,0 });
        localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.y), { 0,1,0 });
        localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.z), { 0,0,1 });
        localMatrix = glm::scale(localMatrix, scale);
    }

    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) : position(pos), rotationXYZ(rot),scale(scl), parent(nullptr) {}

    Transform(glm::mat4 localTransformMatrix) : position(glm::vec3(0.0)), rotationXYZ(glm::vec3(0.0)), scale(glm::vec3(1.0)), parent(nullptr)
    {
        localMatrix = localTransformMatrix;
    }

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

    void setTranslatation(const glm::vec3 trans)
    {
        localMatrix = glm::translate(localMatrix,trans);

        //globalMatrix = localMatrix * parent->getTransformationMatrix();
    }
    void setRotation(const glm::vec3 rot)
    {
        localMatrix = glm::rotate(localMatrix, glm::radians(rot.x), {1,0,0});
        localMatrix = glm::rotate(localMatrix, glm::radians(rot.y), {0,1,0});
        localMatrix = glm::rotate(localMatrix, glm::radians(rot.z), {0,0,1});

        globalMatrix = localMatrix * parent->getTransformationMatrix();
    }
    void setScale(const glm::vec3 scale)
    {
        localMatrix = glm::scale(localMatrix,scale);
        globalMatrix = localMatrix * parent->getTransformationMatrix();
    }

    /*void setScale(const glm::vec3& s) {
        scale = s;
        dirty = true;
    }

    void setRotation(const glm::vec3& r) {
        rotationXYZ = r;
        dirty = true;
    }*/
    void start()
    {
        if(parent == nullptr) 
        {
            globalMatrix = localMatrix;
        }
        else
        {
            globalMatrix = parent->getTransformationMatrix() * localMatrix;
        }
    }

    void update()
    {
        if(parent == nullptr) 
        {
            globalMatrix = localMatrix;
        }
        else
        {
            globalMatrix = parent->getTransformationMatrix() * localMatrix;
        }
    }

    glm::mat4 getTransformationMatrix() const {
        // if (dirty) {
        //     localMatrix = glm::mat4(1.0f);
        //     localMatrix = glm::translate(localMatrix, position);
        //     localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.x), {1,0,0});
        //     localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.y), {0,1,0});
        //     localMatrix = glm::rotate(localMatrix, glm::radians(rotationXYZ.z), {0,0,1});
        //     localMatrix = glm::scale(localMatrix, scale);
        //     dirty = false;
        // }
        return globalMatrix;
    }
};

struct GameObjectData
{
    std::string name;
};
class GameObject{

    public :
    std::string name;
    Transform transform;
    Scene* scene;
    GameObject* parent;
    std::vector<GameObject*> childGameObjects;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    std::vector<std::function<void()>> awakeCallbacks;
    std::vector<std::function<void()>> startCallbacks;
    std::vector<std::function<void()>> updateCallbacks;

    GameObject(const std::string name,Scene* scene, GameObject* parent,glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 rotationXYZ = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f));

    GameObject(const std::string name,Scene* scene, GameObject* parent,Transform transform);
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