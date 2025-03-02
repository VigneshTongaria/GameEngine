#ifndef COMPONENT_H
#define COMPONENT_H
#include<iostream>

class GameObject;

class Component {
public:
    class GameObject* gameObject = nullptr;
};
#endif // !COMPONENT_H