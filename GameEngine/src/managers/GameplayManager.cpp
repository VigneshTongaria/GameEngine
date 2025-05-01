#include"GameplayManager.hpp"

GameplayManager::GameplayManager(/* args */)
{
    
}

void GameplayManager::awake()
{
    for(auto& fun : awakeCallbacks)
    {
        fun();
    }
}

void GameplayManager::start()
{
    for(auto& fun : startCallbacks)
    {
        fun();
    }
}

void GameplayManager::update()
{
    for(auto& fun : updateCallbacks)
    {
        fun();
    }
}

void GameplayManager::AddAwakeCallback(const std::function<void()>& func)
{
    awakeCallbacks.push_back(func);
}

void GameplayManager::AddStartCallback(const std::function<void()>& func)
{
    startCallbacks.push_back(func);
}

void GameplayManager::AddUpdateCallback(const std::function<void()>& func)
{
    updateCallbacks.push_back(func);
}

GameplayManager::~GameplayManager()
{
}