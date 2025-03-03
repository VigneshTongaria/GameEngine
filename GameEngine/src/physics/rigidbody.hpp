#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
#include "Component.h"
#include <glm/glm.hpp>
#include "GameObject.h"

class Rigidbody : public Component
{
private:
     glm::vec3* position;
public:
     float mass;
     float velocity;
     float accelaration;
     Rigidbody(float mass = 1.0f,float velocity = 0.0f,float accelaration = 0.0f);
     void AddForce(glm::vec3 force);

     //Call Start after initialzing all classes and gamobjects
     void Start();

     // Call update in while loop
     void Update(float dt);
};


#endif