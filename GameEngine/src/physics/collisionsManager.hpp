#ifndef COLLISIONSMANAGER_HPP
#define COLLISIONSMANAGER_HPP
#include<iostream>
#include <unordered_map>
#include "rigidbody.hpp"
#include "Colliders/sphereCollider.hpp"
#include "./UtilitiesManager.hpp"
#include "PhysicsCalculations/physicsCollision.hpp"

struct Grid
{
    int x,y;
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
public:
    static constexpr float PhysicsDeltaTime = 0.02f;
    static constexpr float collisionCooldown = 0.2f; // seconds
    static constexpr int GRID_WIDTH = 30;
    static constexpr int GRID_HEIGHT = 30;

    static void AddColliderToPhysics(Collider* c);
    static void TryResolveCollision(Collider *a, Collider *b);
    static void Update();

private:
    static std::unordered_map<UnorderedMapKey<Collider *>, float, UnorderedMapHash<Collider *>> activeCollisions;
    static std::vector<Collider *> sceneColliders;
    static std::vector<Collider *> gridColliders[GRID_WIDTH][GRID_HEIGHT];

    static void resolveGridColliders();
};

#endif