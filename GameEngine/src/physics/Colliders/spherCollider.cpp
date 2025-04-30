#include "sphereCollider.hpp"

sphereCollider::sphereCollider(const float radius)
{
    this->type = SPHERE;
    this->radius = radius;
}

sphereCollider::~sphereCollider()
{

}