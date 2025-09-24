#include "collider.hpp"
#include "collisionsManager.hpp"
#include "rigidbody.hpp"
#include "../core/GameObject.h"

Collider::Collider(const glm::vec3& offset)
{
    GameObject* g = this->gameObject;
    OnCollisionEnterCallback = nullptr;
    isTrigger = false;

    if (g == nullptr)
        std::cout << "Gameobject for collider is null" << std::endl;

    center = offset;
    CollisionsManager::AddColliderToPhysics(this);
}

void Collider::awake()
{
    rb = this->gameObject->GetComponent<Rigidbody>();

    if (rb != nullptr) hasRigidBody = true;
    else hasRigidBody = false;
}

void Collider::start()
{
    
}

Rigidbody* Collider::getAttachedRigidBody() const
{
    return rb;
}

void Collider::UpdateDeltaPosition(const glm::vec3& delta)
{
    this->gameObject->position += delta;
}

glm::vec3 Collider::GetColliderPosition() const
{
    return this->gameObject->position + center;
}