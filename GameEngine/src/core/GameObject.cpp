#include "GameObject.h"
#include "GameObject.inl"
#include "../managers/GameplayManager.hpp"
#include "Component.h"
#include "scene.h"

GameObject::GameObject(Scene* scene,GameObject* parent, glm::vec3 position, glm::vec3 rotationXYZ, glm::vec3 scale)
{
    transform.position = position;
    transform.rotationXYZ = rotationXYZ;
    transform.scale = scale;
    this->scene = scene;
    this->parent = parent;

    GameplayManager::AddAwakeCallback([this]() {this->awake();});
    GameplayManager::AddStartCallback([this]() {this->start();});
    GameplayManager::AddUpdateCallback([this]() {this->update();});
}

GameObject::GameObject(Scene* scene,GameObject* parent, Transform transform)
{
    this->transform.position = transform.position;
    this->transform.rotationXYZ = transform.rotationXYZ;
    this->transform.scale = transform.scale;
    this->scene = scene;
    this->parent = parent;

    GameplayManager::AddAwakeCallback([this]() {this->awake();});
    GameplayManager::AddStartCallback([this]() {this->start();});
    GameplayManager::AddUpdateCallback([this]() {this->update();});
}

void GameObject::awake()
{
    for(auto& fun : awakeCallbacks)
    {
        fun();
    }
}

void GameObject::start()
{
    for(auto& fun : startCallbacks)
    {
        fun();
    }
}

void GameObject::update()
{
    for(auto& fun : updateCallbacks)
    {
        fun();
    }
}