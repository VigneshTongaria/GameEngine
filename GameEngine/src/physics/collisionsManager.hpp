#ifndef COLLISIONSMANAGER_HPP
#define COLLISIONSMANAGER_HPP
#include<iostream>
#include <unordered_map>
#include "rigidbody.hpp"
#include "Colliders/sphereCollider.hpp"

struct Grid
{
    int x,y;
};
struct CollisionInfo
{
    CollisionInfo()
    {
       isCollided = false;
    }

    bool isCollided;
};

static class CollisionsManager
{
private:
    
public:
  
   static CollisionInfo HandleSphereToSphereCollision(sphereCollider* s1,sphereCollider* s2);
    

};

#endif