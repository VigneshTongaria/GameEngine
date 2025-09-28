#pragma once
#include "Camera.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include<iostream>
#include<vector>
#include<../data/GeometryData.hpp>
#include "../managers/ResourcesManager.hpp"
#include "../rendering/FrameBuffers/ShadowRenderTarget.h"
#include "../rendering/FrameBuffers/MSAARenderTarget.h"
#include "GameObject.h"
#include "../rendering/Model.h"
#include "../rendering/Light/DirLight.h"
class Scene
{
private:
    /* data */
public:
    Scene(int width,int height);
    void init();
    
    unsigned int quadVBO,quadVAO;
    unsigned int skyboxVAO,skyboxVBO;
    unsigned int SRC_WIDTH,SRC_HEIGHT;
    MSAARenderTarget mSSARenderTarget;
    std::vector<GameObject> gameObjects;
    std::vector<Model*> sceneModels;
    std::vector<DirLight*> dirLights;
    CubeMap skybox;
    Camera mainCamera;

    void render();
    void draw(Shader* shader);

    void setViewAndProjectionForAllShaders(unsigned int uboIndex);
 
    ~Scene();
};

