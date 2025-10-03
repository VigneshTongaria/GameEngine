#include "UtilitiesManager.hpp"
#include <iostream>

glm::mat4 UtilitiesManger::convertToGLM (const aiMatrix4x4& from)
{
    glm::mat4 to;

    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

    return to;
}

void UtilitiesManger::mattrixDebugger(const glm::mat4 mat)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout << mat[row][col] << " ";  // glm is column-major, so mat[col][row]
        }
        std::cout << std::endl;
    }
}

void decomposeTRS(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale)
{
    // Extract translation directly
    pos = glm::vec3(m[3]); // last column

    // Extract scale: length of basis vectors
    scale.x = glm::length(glm::vec3(m[0]));
    scale.y = glm::length(glm::vec3(m[1]));
    scale.z = glm::length(glm::vec3(m[2]));

    // Normalize the rotation matrix columns (remove scale)
    glm::mat3 rotMat;
    rotMat[0] = glm::vec3(m[0]) / scale.x;
    rotMat[1] = glm::vec3(m[1]) / scale.y;
    rotMat[2] = glm::vec3(m[2]) / scale.z;
}