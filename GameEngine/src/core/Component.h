#pragma once
#include<iostream>

class GameObject;
class DirLight;

class Component {
public:

    Component();
    GameObject* gameObject = nullptr;

    virtual void awake() ;
    virtual void start();
    virtual void fixedUpdate(float dt);
    virtual void update();
};