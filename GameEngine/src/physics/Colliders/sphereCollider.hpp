#pragma once
#include "../collider.hpp"
#include "../collisionsManager.hpp"

class sphereCollider : public Collider
{
private:
    /* data */
protected:
    
public:
    float radius;
    sphereCollider(const float radius = 1.0f);
    ~sphereCollider();
};