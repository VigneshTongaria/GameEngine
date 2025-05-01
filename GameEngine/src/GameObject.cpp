#include "GameObject.h"
#include "managers/GameplayManager.hpp"

GameObject::GameObject(glm::vec3 position, glm::vec3 rotationXYZ, glm::vec3 scale)
{
    this->position = position;
    this->rotationXYZ = rotationXYZ;
    this->scale = scale;

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