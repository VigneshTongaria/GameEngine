#include "rigidbody.hpp"

Rigidbody::Rigidbody(float mass, glm::vec3 velocity, glm::vec3 accelaration)
{
    this->mass = mass; 
    this->velocity = velocity;
    this->accelaration = accelaration;

    this->position = nullptr;
}

void Rigidbody::start()
{
    position = &this->gameObject->position;
}

void Rigidbody::Update(float dt)
{
    glm::vec3 ds = velocity*dt + (accelaration * dt * dt) / 2.0f;
    *position += ds;
    velocity += accelaration * dt;
}

void Rigidbody::AddForce(glm::vec3 force)
{
    accelaration += force / mass;
}

void Rigidbody::AddImpulse(glm::vec3 impulse)
{
    velocity += impulse/mass;
}