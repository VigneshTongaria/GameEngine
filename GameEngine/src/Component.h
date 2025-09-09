#ifndef COMPONENT_H
#define COMPONENT_H
#include<iostream>
#include"GameObject.h"

class Component {
public:

    Component();
    GameObject* gameObject = nullptr;

    virtual void awake();
    virtual void start();
    virtual void fixedUpdate(float dt);
    virtual void update();
};

Component::Component()
{
    if (gameObject != nullptr)
    {
        gameObject->awakeCallbacks.push_back([this]() { this->awake(); });
        gameObject->startCallbacks.push_back([this]() { this->start(); });
    }
}


#endif // !COMPONENT_H