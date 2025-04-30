#ifndef UTILITIESMANAGER_HPP
#define UTILITIESMANAGER_HPP
#include "../Mesh.h"

template<typename T>
struct UnorderedMapKey
{
    T a,b;

    UnorderedMapKey(T a,T b)
    {
        this->a = a;
        this->b = b;
    }

    bool operator==(const T& other)
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
};

#endif