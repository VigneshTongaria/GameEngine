#ifndef COLLLIDER_H
#define COLLIDER_H
#include<iostream>
#include<glm/glm.hpp>
#include "./Component.h"
#include "./GameObject.h"
#include <functional>
#include "collisionsManager.hpp"

enum ColliderType
{
    BOX,SPHERE
};


class Collider : public Component
{

protected :

public:
    
    ColliderType type;
    glm::vec3 center;
    bool isTrigger;

    std::function<void(Collider* other)> OnCollisionEnterCallback;

    Collider(const glm::vec3& offset = glm::vec3(0.0f));
    virtual ~Collider() = default;

    virtual void UpdatePosition(const glm::vec3& offset);

    glm::vec3 GetColliderPosition() const;
};



#endif

Collider::Collider(const glm::vec3& offset)
{
    GameObject *g = this->gameObject;
    OnCollisionEnterCallback= nullptr;
    isTrigger = false;

    if (g == nullptr)
        std::cout << "Gameobject for collider is null" << std::endl;

    center = offset;
}

void Collider::UpdatePosition(const glm::vec3& offset)
{
    center += offset;
}

glm::vec3 Collider::GetColliderPosition() const
{
    return this->gameObject->position + center;
}