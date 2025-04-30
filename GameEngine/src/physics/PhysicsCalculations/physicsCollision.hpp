#include"./collisionsManager.hpp"
#include"././UtilitiesManager.hpp"

// struct CollisionKey
// {
//     ColliderType a,b;

//     bool operator==(const CollisionKey& other)
//     {
//         return ((a == other.a && b == other.b) || (a == other.b && b == other.a));
//     }
// };

// struct CollisionKeyHash
// {
//     std::size_t operator()(const CollisionKey& pair) const {
//         auto ha = std::hash<ColliderType>()(pair.a);
//         auto hb = std::hash<ColliderType>()(pair.b);
//         return ha ^ hb; // XOR works fine for pointer hashing
//     }
// };

static class physicsCollision
{
private:
    /* data */
    static void ResolveSphereToSphereCollision(Collider* s1,Collider* s2);

public:

    physicsCollision(/* args */);

    using CollisionKey = UnorderedMapKey<ColliderType>;
    using CollisionKeyHash = UnorderedMapHash<ColliderType>;

    using CollisionFunc = void(*)(Collider*, Collider*);
    static std::unordered_map<CollisionKey, CollisionFunc, CollisionKeyHash> collisionMap;

    static void ResolveCollisions(Collider* c1,Collider* c2);

    ~physicsCollision();
};
