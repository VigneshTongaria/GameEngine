#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include"physics/collisionsManager.hpp"
#include"physics/rigidbodyManager.hpp"

void startPhysicsThread();
void stopPhysicsThread();

std::thread PhysicsThread;
std::atomic<bool> physicsRunning = false;
std::mutex physicsMutex;
const static long long physicsDelta = 0.02f;

void physicsMain()
{
    while(physicsRunning)
    {
        std::lock_guard<std::mutex> lock(physicsMutex);
        CollisionsManager::Update();
        RigidbodyManager::Update(physicsDelta);

        std::this_thread::sleep_for(std::chrono::seconds(physicsDelta));
    }
}

void startPhysicsThread()
{
    physicsRunning = true;
    PhysicsThread = std::thread(physicsMain);
}

void stopPhysicsThread() {

    physicsRunning = false;

    if (PhysicsThread.joinable())
        PhysicsThread.join();
}
