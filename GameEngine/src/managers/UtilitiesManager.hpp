#pragma once

#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>

template<typename T>
struct UnorderedMapKey
{
    T a,b;

    UnorderedMapKey(T a,T b)
    {
        this->a = a;
        this->b = b;
    }

    bool operator==(const UnorderedMapKey<T>& other) const
    {
        return ((a == other.a && b == other.b) || (a == other.b && b == other.a));
    }
};

template<typename T>
struct UnorderedMapHash
{
    std::size_t operator()(const UnorderedMapKey<T>& pair) const {
        auto ha = std::hash<T>()(pair.a);
        auto hb = std::hash<T>()(pair.b);
        return ha ^ hb; // XOR works fine for pointer hashing
    }
};

class UtilitiesManger
{
private:

public:
    static glm::mat4 convertToGLM(const aiMatrix4x4& from);
    static void mattrixDebugger(const glm::mat4 mat);
    static void decomposeTRS(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale);
};