#ifndef COMPONENT_H
#define COMPONENT_H
#include<iostream>

class GameObject;

class Component {
public:

    Component();
    GameObject* gameObject = nullptr;

    virtual void awake();
    virtual void start();
    virtual void update();
};

Component::Component()
{
    gameObject->awakeCallbacks.push_back([this]() { this->awake(); });
    gameObject->startCallbacks.push_back([this]() { this->start(); });
}


#endif // !COMPONENT_H