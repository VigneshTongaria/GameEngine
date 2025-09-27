#pragma once
#include "Camera.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<../data/GeometryData.hpp>
#include "../managers/ResourcesManager.hpp"

class Scene
{
private:
    /* data */
public:
    Scene(/* args */);
    void init();
    
    unsigned int SRC_WIDTH = 1920,SRC_HEIGHT = 1080;
    Camera mainCamera;
 
    ~Scene();
};

