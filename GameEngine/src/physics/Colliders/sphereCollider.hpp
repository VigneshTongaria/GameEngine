#ifndef SPHERECOLLIDER_HPP
#define SPHERECOLLIDER_HPP
#include "./collider.hpp"
#include "./collisionsManager.hpp"

class sphereCollider : public Collider
{
private:
    /* data */
public:
    float radius;
    sphereCollider(const float radius = 1.0f);
    ~sphereCollider();
};
#endif