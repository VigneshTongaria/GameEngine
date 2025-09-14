#include "rigidbodyManager.hpp"

std::vector<Rigidbody*> RigidbodyManager::m_sceneRigidbodies;

RigidbodyManager::RigidbodyManager(/* args */)
{
}

void RigidbodyManager::AddrigidbodyToPhysics(Rigidbody* rb)
{
    m_sceneRigidbodies.push_back(rb);
}

void RigidbodyManager::Update(float dt)
{
    for (auto& rb : m_sceneRigidbodies)
    {
        rb->fixedUpdate(dt);
    }
}

RigidbodyManager::~RigidbodyManager()
{
}