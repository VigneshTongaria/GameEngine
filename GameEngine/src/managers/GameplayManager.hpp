#pragma once
#include<iostream>
#include "functional"

static class GameplayManager
{
private:
    static std::vector<std::function<void()>> awakeCallbacks;
    static std::vector<std::function<void()>> startCallbacks;
    static std::vector<std::function<void()>> updateCallbacks;

public:
    GameplayManager(/* args */);
    
    // Call awake for initializing all gameobejects
    static void awake();

    // Call start after all gameobjects are initalized
    static void start();

    // Call update in the while for all gameobjects
    static void update();
    
    // Add function to be called on awake method of main gameloop
    static void AddAwakeCallback(const std::function<void()>& awakefunc);

    // Add function to be called on start method of main gameloop
    static void AddStartCallback(const std::function<void()>& startfunc);

    // Add function to be called on update method of main gameloop
    static void AddUpdateCallback(const std::function<void()>& updatefunc);

    ~GameplayManager();
};