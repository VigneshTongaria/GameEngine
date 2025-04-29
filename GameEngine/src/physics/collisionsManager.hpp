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

struct CollisionPair {
    Collider* a;
    Collider* b;

    bool operator==(const CollisionPair& other) const {
        return (a == other.a && b == other.b) || (a == other.b && b == other.a);
    }
};

// Hash function for unordered_set
struct CollisionPairHasher {
    std::size_t operator()(const CollisionPair& pair) const {
        auto ha = std::hash<Collider*>()(pair.a);
        auto hb = std::hash<Collider*>()(pair.b);
        return ha ^ hb; // XOR works fine for pointer hashing
    }
};

static class CollisionsManager
{
private:
static std::unordered_map<CollisionPair, float, CollisionPairHasher> activeCollisions;
    
public:
   static constexpr float collisionCooldown = 0.2f; // seconds
  
   static CollisionInfo HandleSphereToSphereCollision(sphereCollider* s1,sphereCollider* s2);

   static void TryResolveCollision(Collider* a, Collider* b, float deltaTime) {
    CollisionPair pair = { a, b };
    
    auto it = activeCollisions.find(pair);
    if (it != activeCollisions.end()) {
        // Decrease cooldown
        it->second -= deltaTime;
        if (it->second <= 0.0f) {
            activeCollisions.erase(it); // Can resolve again next time
        }
        return; // Already resolved recently
    }

    // Not in set → resolve it
    //Physics::ResolveCollision(a, b);
    activeCollisions[pair] = collisionCooldown;
}

static void Update(float deltaTime) {
    for (auto it = activeCollisions.begin(); it != activeCollisions.end(); ) {
        it->second -= deltaTime;
        if (it->second <= 0.0f)
            it = activeCollisions.erase(it);
        else
            ++it;
    }
}

};

#endif