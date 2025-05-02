#include "physicsCollision.hpp"

physicsCollision::physicsCollision()
{
    collisionMap[CollisionKey(ColliderType::SPHERE,ColliderType::SPHERE)] = ResolveSphereToSphereCollision;
}

bool physicsCollision::ResolveCollisions(Collider* c1,Collider* c2)
{
    return collisionMap[CollisionKey(c1->type,c2->type)](c1,c2);
}

bool physicsCollision::ResolveSphereToSphereCollision(Collider* c1, Collider*c2)
{
    sphereCollider* s1 = static_cast<sphereCollider*>(c1);
    sphereCollider* s2 = static_cast<sphereCollider*>(c2);

    std::cout<<"Resolving two sphere colliders"<<"\n";

    float combinedRadius = s1->radius + s2->radius; 

    float distanceDelta = glm::length(s1->GetColliderPosition() - s2->GetColliderPosition());

    if(distanceDelta > combinedRadius)
       return false;
    
    
    
}