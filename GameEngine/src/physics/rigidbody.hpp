#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
#include "../GameObject.h"
#include <glm/glm.hpp>

class Rigidbody : public Component
{
private:
     glm::vec3* position;
public:
     float mass;
     glm::vec3 velocity;
     glm::vec3 accelaration;
     Rigidbody(float mass = 1.0f,glm::vec3 velocity = glm::vec3(1.0f), glm::vec3 accelaration = glm::vec3(1.0f));

     void AddImpulse(glm::vec3 impulse);
     void AddForce(glm::vec3 force);

     void start() override;

     // Call update in while loop
     void Update(float dt);
};


#endif