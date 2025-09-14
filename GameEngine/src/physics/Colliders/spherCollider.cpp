#include "sphereCollider.hpp"

sphereCollider::sphereCollider(const float radius)
{
    this->type = ColliderType::SPHERE;
    this->radius = radius;
}

sphereCollider::~sphereCollider()
{

}