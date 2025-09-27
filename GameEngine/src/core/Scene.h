#pragma once
#include "Camera.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<../data/GeometryData.hpp>
#include "../managers/ResourcesManager.hpp"
#include "../rendering/FrameBuffers/ShadowRenderTarget.h"
#include "../rendering/FrameBuffers/MSAARenderTarget.h"
#include "GameObject.h"
#include "../rendering/Model.h"

class Scene
{
private:
    /* data */
public:
    Scene(int width,int height);
    void init();
    
    unsigned int quadVBO,quadVAO;
    unsigned int skyboxVAO,skyboxVBO;
    unsigned int SRC_WIDTH = 1920,SRC_HEIGHT = 1080;
    MSAARenderTarget mSSARenderTarget;
    std::vector<GameObject> gameObjects;
    std::vector<Model*> sceneModels;
    Camera mainCamera;
 
    ~Scene();
};

