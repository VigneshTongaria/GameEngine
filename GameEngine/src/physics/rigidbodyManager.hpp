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
