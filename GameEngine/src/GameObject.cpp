#include "GameObject.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 rotationXYZ, glm::vec3 scale)
{
    this->position = position;
    this->rotationXYZ = rotationXYZ;
    this->scale = scale;
}