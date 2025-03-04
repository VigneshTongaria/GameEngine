#include "rigidbody.hpp"

Rigidbody::Rigidbody(float mass, glm::vec3 velocity, glm::vec3 accelaration)
{
    this->mass = mass; 
    this->velocity = velocity;
    this->accelaration = accelaration;

    this->position = nullptr;
}

void Rigidbody::Start()
{
    position = &this->gameObject->position;
}

void Rigidbody::Update(float dt)
{
    *position += velocity*dt + (accelaration * dt * dt) / 2.0f;
}

void Rigidbody::AddForce(glm::vec3 force)
{
    accelaration += force / mass;
}

void Rigidbody::AddImpulse(glm::vec3 impulse)
{
    velocity += impulse/mass;
}