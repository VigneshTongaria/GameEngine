#pragma once
#include<iostream>
#include<vector>
#include"rigidbody.hpp"

static class RigidbodyManager
{
private:
    /* data */
    static std::vector<Rigidbody*> m_sceneRigidbodies;
public:
    RigidbodyManager(/* args */);

    static void AddrigidbodyToPhysics(Rigidbody* rb);
    static void Update(float dt);

    ~RigidbodyManager();
};

RigidbodyManager::RigidbodyManager(/* args */)
{
}

void RigidbodyManager::AddrigidbodyToPhysics(Rigidbody* rb)
{
    m_sceneRigidbodies.push_back(rb);
}

void RigidbodyManager::Update(float dt)
{
    for(auto& rb : m_sceneRigidbodies)
    {
        rb->fixedUpdate(dt);
    }
}

RigidbodyManager::~RigidbodyManager()
{
}
