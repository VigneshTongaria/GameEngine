#pragma once
#include<iostream>
#include<glm/glm.hpp>
#include "../core/Component.h"
#include <functional>

enum class ColliderType
{
    BOX,SPHERE
};

class Rigidbody;

class Collider : public Component
{

protected :

public:
    
    ColliderType type;
    glm::vec3 center;
    bool isTrigger;
    bool hasRigidBody;
    Rigidbody *rb;

    void awake() override;
    void start() override;

    Rigidbody* getAttachedRigidBody() const;

    std::function<void(Collider* other)> OnCollisionEnterCallback;

    Collider(const glm::vec3& offset = glm::vec3(0.0f));
    virtual ~Collider() = default;

    virtual void UpdateDeltaPosition(const glm::vec3& newPosition);

    glm::vec3 GetColliderPosition() const;
};