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
#include "ComponentsType.h"
#include "../rendering/FrameBuffers/MSAARenderTarget.h"
#include "../managers/ResourcesManager.hpp"

class Component;
class GameObject;
class Model;
class DirLight;
class CubeMap;
class Camera;
class Shader;
struct Transform;
class MeshRenderer;

class Scene
{
private:
public:
    Scene(int width,int height);
    void init();
    
    unsigned int quadVBO,quadVAO;
    unsigned int skyboxVAO,skyboxVBO;
    unsigned int uboMatrices;
    unsigned int SRC_WIDTH,SRC_HEIGHT;

    float cameraSpeed = 0.1f;
    float Arrow_vertical_Input = 0.0f;
    glm::mat4 Scale = glm::mat4(1.0f);
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    MSAARenderTarget mSSARenderTarget;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::unordered_map<COMPONENT_TYPE,std::vector<std::shared_ptr<Component>>> componentsMap;

    template<typename T> 
    void addComponentToScene(std::shared_ptr<T> comp);

    template<typename T = void,typename...Args> 
    GameObject* addNewGameObjectToScene(GameObject* parent,Transform transform, Args&&... args);

    std::vector<Model*> sceneModels;
    std::vector<MeshRenderer*> sceneMeshRenderers;
    std::vector<DirLight*> dirLights;
    CubeMap skybox;
    Camera mainCamera;
    Shader* depthMapShader;
    Shader* lightingShadowShader;
    
    void start();
    void render();
    void update();
    void draw(Shader* shader);
    void process_inputs();

    void setViewAndProjectionForAllShaders(unsigned int uboIndex);
    void updateSceneComponentsType(COMPONENT_TYPE type);
 
    ~Scene();
};

