#include "Component.h"
#include "GameObject.h"

Component::Component()
{
    if (gameObject != nullptr)
    {
        gameObject->awakeCallbacks.push_back([this]() { this->awake(); });
        gameObject->startCallbacks.push_back([this]() { this->start(); });
    }
}

void Component::awake() {}
void Component::start() {}
void Component::fixedUpdate(float dt) {}
void Component::update() {}