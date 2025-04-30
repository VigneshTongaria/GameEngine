#include "physicsCollision.hpp"

physicsCollision::physicsCollision()
{
    collisionMap[CollisionKey(ColliderType::SPHERE,ColliderType::SPHERE)] = ResolveSphereToSphereCollision;
}

void physicsCollision::ResolveCollisions(Collider* c1,Collider* c2)
{
    collisionMap[CollisionKey(c1->type,c2->type)](c1,c2);
}

void physicsCollision::ResolveCollisions(Collider* c1, Collider*c2)
{
    sphereCollider* s1 = static_cast<sphereCollider*>(c1);
    sphereCollider* s2 = static_cast<sphereCollider*>(c2);
}