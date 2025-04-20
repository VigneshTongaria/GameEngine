#ifndef UTILITIESMANAGER_HPP
#define UTILITIESMANAGER_HPP
#include "../Mesh.h"

class UtilitiesManger
{
private:

public:
    static glm::mat4 convertToGLM(const aiMatrix4x4& from);
};

#endif