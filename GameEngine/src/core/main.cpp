#include<iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include "../rendering/Shader.h"
#include <stb/stb_image.h>
#include "../io/KeyBoard.h"
#include "../io/Mouse.h"
#include "Camera.h"
#include "../rendering/Model.h"
#include "GameObject.h"
#include "../physics/rigidbody.hpp"
#include "../managers/ResourcesManager.hpp"
#include "../data/GeometryData.hpp"
#include "../physics/PhysicsThread.h"
#include "../managers/ShaderManager.hpp"
#include "Scene.h"
#include "../GUI/HierarchyPanel.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);
void SetViewAndProjectionForAllShaders(unsigned int uboIndex);
void RenderScene(Shader* shader,std::vector<Model*> models,int cubeVAO);
void RenderAsteriods(Model* m,Shader* s);
float Arrow_vertical_Input = 0.0f;

glm::mat4 Scale = glm::mat4(1.0f);

float cameraSpeed = 0.1f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
unsigned int SRC_HEIGHT = 1080;
unsigned int SRC_WIDTH = 1920;

Camera MainCamera;

// Asteriods mattrices
std::vector<glm::mat4> asteriodModelMats;

// All models
std::vector<Model*> SceneModels;

// All shaders
std::vector<Shader*> shaders;
std::vector<std::string> cubeFaces
{
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/right.jpg",
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/left.jpg",
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/top.jpg",
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/bottom.jpg",
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/front.jpg",
    "C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/skybox/back.jpg"
};


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	
	glViewport(0, 0,SRC_WIDTH,SRC_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window,KeyBoard::KeyCallback);
	glfwSetMouseButtonCallback(window,Mouse::MouseButtonCallback);
	glfwSetCursorPosCallback(window,Mouse::CursorButtonCallback);
	glfwSetScrollCallback(window,Mouse::MouseScrollCallback);

	

	// Generating Frame buffers

	// Multisampling buffers
	// unsigned int msbo;
	// glGenFramebuffers(1,&msbo);
	// glBindFramebuffer(GL_FRAMEBUFFER,msbo);

	// unsigned int textureColorBufferMultiSampled;
    // glGenTextures(1, &textureColorBufferMultiSampled);
    // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB,SRC_WIDTH, SRC_HEIGHT, GL_TRUE);
    // glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
	// glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMultiSampled,0);

	// // Multisample Renderbuffers
	// unsigned int mrbo;
	// glGenRenderbuffers(1,&mrbo);
	// glBindRenderbuffer(GL_RENDERBUFFER,mrbo);
	// glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,SRC_WIDTH, SRC_HEIGHT);

	// glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,mrbo);
	// if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//   std::cout<<"Frame buffers not generated"<<std::endl;
	
	// glBindRenderbuffer(GL_RENDERBUFFER,0);
	// glBindFramebuffer(GL_FRAMEBUFFER,0);

	// // Post processing Frame buffer
	// unsigned int fbo;
	// glGenFramebuffers(1,&fbo);
	// glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	// Texture colorBuffer = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	// Texture brightColorBuffer = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	// glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorBuffer.id,0);
	// glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,brightColorBuffer.id,0);

	// unsigned int attachMents[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	// glDrawBuffers(2,attachMents);

	// // Blur frameBuffer
	// unsigned int pingpongFBO[2];
	// glGenFramebuffers(2, pingpongFBO);
	// Texture pingpongBuffers[2];
	// pingpongBuffers[0] = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	// pingpongBuffers[1] = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);

	// for (unsigned int i = 0; i < 2; i++)
	// {
	// 	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
	// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i].id, 0);
	// }

	// // Post processing renderBuffers
	// unsigned int rbo;
	// glGenRenderbuffers(1,&rbo);
	// glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	// glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,SRC_WIDTH, SRC_HEIGHT);

	// glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	// if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//   std::cout<<"Frame buffers not generated"<<std::endl;
	
	// glBindRenderbuffer(GL_RENDERBUFFER,0);
	// glBindFramebuffer(GL_FRAMEBUFFER,0);

	// // Generating directional light depth map buffers

	// unsigned int depthMapFBO;
	// glGenFramebuffers(1, &depthMapFBO);
	// glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);

	// const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	// unsigned int depthMap;
	// float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glGenTextures(1, &depthMap);
	// glBindTexture(GL_TEXTURE_2D, depthMap);
	// glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	// glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthMap,0);
	// glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// // Generating point light cubemaps depth buffers

	// std::vector<unsigned int> pointLightDepthMap;
	// std::vector<unsigned int> pointLightDepthMapBuffers;

	// for(unsigned int i = 0; i<NR_POINT_LIGHTS; i++)
	// {
    //     unsigned int depthCubeMap;
	// 	glGenTextures(1, &depthCubeMap);
	// 	unsigned int depthBuffer;
	// 	glGenFramebuffers(1,&depthBuffer);
	// 	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

	// 	for(unsigned int j=0 ; j<6 ; j++)
	// 	{
	// 		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	// 	}
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// 	glBindFramebuffer(GL_FRAMEBUFFER,depthBuffer);
	// 	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap,0);
	// 	glDrawBuffer(GL_NONE);
	// 	glReadBuffer(GL_NONE);
	// 	glBindFramebuffer(GL_FRAMEBUFFER,0);
	// 	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// 	pointLightDepthMap.push_back(depthCubeMap);
	// 	pointLightDepthMapBuffers.push_back(depthBuffer);
	// }

	// // Generating uniform buffers
	// unsigned int uboMatrices;
	// glGenBuffers(1, &uboMatrices);

	// glBindBufferRange(GL_UNIFORM_BUFFER,0, uboMatrices, 0, 3 * sizeof(glm::mat4));

	// glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	// glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	// glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
    
	// GameObject gameObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(10.0f,30.0f,50.0f),glm::vec3(1.0f,1.0f,1.0f));

    // //	Adding gameObject components for  our model
	// // gameObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/backpack/backpack.obj");
	// // Model* ourModel = gameObject.GetComponent<Model>();

	// // SceneModels.push_back(ourModel);

	// gameObject.AddComponent<Rigidbody>(1.0f,glm::vec3(0.0f), glm::vec3(0.0f,0.0f,0.0f));
	// Rigidbody* rb = gameObject.GetComponent<Rigidbody>();

	// // Adding entire city
	// GameObject cityObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-90.0f,0.0f,0.0f),glm::vec3(0.1f,0.1f,0.1f));
	// cityObject.AddComponent<Model>
	// ("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/City/City.glb");
	// Model* cityModel = cityObject.GetComponent<Model>();

	// SceneModels.push_back(cityModel);

	// // Adding asteriod belt GameObject
	// GameObject asteriodObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(30.0f,50.0f,10.0f),glm::vec3(1.0f,1.0f,1.0f));
	// asteriodObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/rock/rock.obj",asteriodInstances);

	// Model* asteriodModel = asteriodObject.GetComponent<Model>();

	// // Texture for cubes
	// std::vector<Texture> newTexture;
	// newTexture.push_back( ResourcesManager::loadTexture("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/blending_transparent_window.png",TEXTURE_TYPE::DIFFUSE));

	// // Gameobjects for cubes
	// // GameObject** CubesGameObject = new GameObject*[10];
    // // for(unsigned int i = 0; i < 10; i++)
	// // {
    // //     CubesGameObject[i] = new GameObject(cubePositions[i]);
	// // 	CubesGameObject[i]->AddComponent<Model>(DEFAULT_MODEL::CUBE,newTexture);
	// // }

	// // Adding light projection and view matrices
	// float near_plane = 1.0f,far_plane = 300.5f;
	// glm::vec3 DirectionalLightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	// glm::mat4 lightView = glm::lookAt(-10.0f* DirectionalLightDir,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	// glm::mat4 lightProj = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane, far_plane);
	// glm::mat4 lightSpaceMatrix =  lightProj *lightView;

	// points light view and projection matrix

	// float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	// float near = 1.0f;
	// float far = 25.0f;
	// glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	// std::vector<std::vector<glm::mat4>> pointLightsViewProjection;

	// for(unsigned int i=0;i<NR_POINT_LIGHTS;i++)
	// {
	// 	glm::vec3 lightPos = pointLightPositions[i];
	// 	std::vector<glm::mat4> newLightView;
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	// 	newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	// 	// for (int i = 0; i < 6; ++i) {
	// 	// 	std::cout << "Matrix " << i << ":\n" << glm::to_string(newLightView[i]) << std::endl;
	// 	// }
	// 	pointLightsViewProjection.push_back(newLightView);
	// }

	// Loading cubeMap

	// CubeMap cubeMap = ResourcesManager::loadCubeMap(cubeFaces);
	// CubeMapShader.UseShaderProgram();
	// CubeMapShader.setInt("skybox",0);
    
	// LightingShader.UseShaderProgram();

	// // Setting reflection probe
	// LightingShader.setInt("reflection",0);
	// LightingShader.setFloat("material.shininess",32.0f);

	// LightingShader.setVec3("dirLight.direction", DirectionalLightDir);
	// LightingShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	// LightingShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
	// point light 1
	// LightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	// LightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	// LightingShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	// LightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setFloat("pointLights[0].constant", 1.0f);
	// LightingShader.setFloat("pointLights[0].linear", 0.09f);
	// LightingShader.setFloat("pointLights[0].quadratic", 0.032f);
	// // point light 2
	// LightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	// LightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	// LightingShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	// LightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setFloat("pointLights[1].constant", 1.0f);
	// LightingShader.setFloat("pointLights[1].linear", 0.09f);
	// LightingShader.setFloat("pointLights[1].quadratic", 0.032f);
	// // point light 3
	// LightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	// LightingShader.setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	// LightingShader.setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	// LightingShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setFloat("pointLights[2].constant", 1.0f);
	// LightingShader.setFloat("pointLights[2].linear", 0.09f);
	// LightingShader.setFloat("pointLights[2].quadratic", 0.032f);
	// // point light 4
	// LightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	// LightingShader.setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	// LightingShader.setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	// LightingShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setFloat("pointLights[3].constant", 1.0f);
	// LightingShader.setFloat("pointLights[3].linear", 0.09f);
	// LightingShader.setFloat("pointLights[3].quadratic", 0.032f);
	// // spotLight

	// LightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	// LightingShader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShader.setFloat("spotLight.constant", 1.0f);
	// LightingShader.setFloat("spotLight.linear", 0.09f);
	// LightingShader.setFloat("spotLight.quadratic", 0.032f);
	// LightingShader.setFloat("spotLight.cosTheta", glm::cos(glm::radians(12.5f)));
	// LightingShader.setFloat("spotLight.cosThetaOuter", glm::cos(glm::radians(15.0f)));

	// Lighting shadow shader values set

	// LightingShadowShader.UseShaderProgram();
	// LightingShadowShader.setFloat("material.shininess",32.0f);
	// LightingShadowShader.setInt("reflection",13);

	// LightingShadowShader.setVec3("dirLight.direction", DirectionalLightDir);
	// LightingShadowShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
	// LightingShadowShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	// LightingShadowShader.setVec3("dirLight.specular", glm::vec3(0.4f, 0.4f, 0.4f));

	// // Setting and binding all the shadow depth maps

	// for(unsigned int i=0 ;i<NR_POINT_LIGHTS; i++)
	// {
	// 	LightingShadowShader.setInt("pointShadowMap["+std::to_string(i)+"]",5 + i);
	// }
	// LightingShadowShader.setFloat("far_plane",far);

	// glBindTexture(GL_TEXTURE_2D,depthMap);
	// LightingShadowShader.setTransformation("mat_Lightspace",lightSpaceMatrix);
	// LightingShadowShader.setInt("shadowMap",10);


	// glBindTexture(GL_TEXTURE_2D,0);

	// // Depth map shader values set
	// DepthMapShader.UseShaderProgram();
	// DepthMapShader.setTransformation("lightSpaceMatrix",lightSpaceMatrix);
    
	// // Cube Shaders 
	// LightnigSourceShader.UseShaderProgram();

	// HighlightShader.UseShaderProgram();

	// ImageShader.UseShaderProgram();

	// //camera

	// // Set projection mattrix in the uniform buffer

	// glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
	// glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(MainCamera.GetProjectionMatrix()));
	// glBindBuffer(GL_UNIFORM_BUFFER,0);

	// // Call all start functions here
    // lastFrame = float(glfwGetTime());
	// rb->start();
	// ResourcesManager::VerticesCount = 0;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window,true);
	ImGui_ImplOpenGL3_Init("#version 460");

	startPhysicsThread();
    ShaderManager::init();
	Scene scene(SRC_WIDTH,SRC_HEIGHT);

	scene.init();
	HierarchyPanel h_Panel(&scene);

	Model cityModel = Model("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/City/City.glb");

	cityModel.addModelToScene(&scene);

	// Start
	scene.start();
	
    
	while (!glfwWindowShouldClose(window))
	{

		scene.update();
		scene.render();
		//calculate deltaTime
		// float currentTime = float(glfwGetTime());
		// deltaTime = currentTime - lastFrame;

		// std::cout<<"--[ Frame Time ] : "<<deltaTime<<std::endl;
		// lastFrame = currentTime;
		// //process inputs
		// process_inputs(window);

		// //rendering
		// ResourcesManager::VerticesCount = 0;
		// SetViewAndProjectionForAllShaders(uboMatrices);

		// // Rendering scene first for depth Map for directional light
		// glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		// glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// //glEnable(GL_CULL_FACE);
		// glEnable(GL_DEPTH_TEST);

		// RenderScene(&DepthMapShader,SceneModels,lightVAO);
        
		// // Rendering scene for all lights
		// // for(unsigned int i=0; i<NR_POINT_LIGHTS; i++)
		// // {
		// // 	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); 
		// // 	glBindFramebuffer(GL_FRAMEBUFFER,pointLightDepthMapBuffers[i]);
		// // 	glClear(GL_DEPTH_BUFFER_BIT);
		// // 	PointLightingShadowShader.UseShaderProgram();
		// // 	PointLightingShadowShader.setVec3("lightPos",pointLightPositions[i]);
		// // 	PointLightingShadowShader.setFloat("far_plane",far);
		// // 	for(unsigned int j=0; j<6; j++)
		// // 	{
		// // 		PointLightingShadowShader.setTransformation("pointLightSpaceView[" + std::to_string(j) + "]",pointLightsViewProjection[i][j]);
		// // 	}
		// // 	RenderScene(&PointLightingShadowShader,SceneModels,lightVAO);
		// // }

		// // Binding framebuffers
		// glBindFramebuffer(GL_FRAMEBUFFER,msbo);

		// // All tests
		// glEnable(GL_DEPTH_TEST);
		// glEnable(GL_STENCIL_TEST);
		// glEnable(GL_BLEND);
		// glEnable(GL_FRAMEBUFFER_SRGB);
		// glEnable(GL_CULL_FACE);

		// glDepthFunc(GL_LEQUAL);
		// glStencilOp(GL_KEEP,GL_REPLACE,GL_REPLACE);
		// glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		// glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);

		// glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// // Disable writing to stencil buffer
		// //glStencilMask(0x00);

 
		// //draw shapes
		// LightingShadowShader.UseShaderProgram();

		// glActiveTexture(GL_TEXTURE10);
		// glBindTexture(GL_TEXTURE_2D,depthMap);

		// // for(unsigned int i=0;i<NR_POINT_LIGHTS;i++)
		// // {
		// // 	glActiveTexture(GL_TEXTURE5 + i);
		// // 	glBindTexture(GL_TEXTURE_CUBE_MAP,pointLightDepthMap[i]);
		// // }

		// float time = static_cast<float>(glfwGetTime());
		
		// //  LightPositions[0].x = 1.0f*glm::sin(glm::radians(time*10.0f));
		// //  LightPositions[0].z = 1.0f*glm::cos(glm::radians(time*10.0f));
		// // LightingShader.setVec3("spotLight.position", MainCamera.GetCameraPos());
		// // LightingShader.setVec3("spotLight.direction", MainCamera.GetCameraFront());

		// LightingShadowShader.setVec3("viewPos",MainCamera.GetCameraPos());
		
		// // for(unsigned int i = 0 ; i<10 ; i++)
		// // {
		// // 	glm::mat4 _model = glm::mat4(1.0f);
		// // 	_model = glm::translate(_model,cubePositions[i]);
        // // 	LightingShader.setTransformation("mat_Model",_model);
		// // 	glDrawArrays(GL_TRIANGLES,0,36);
		// // }
		// // writing stencil on models

		// LightingShadowShader.setInt("hasNormalMap",1);
		
		// glActiveTexture(GL_TEXTURE13);
		// glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap.id);
		// glActiveTexture(GL_TEXTURE0);

		// for(auto& model : SceneModels)
		// {
		// 	model->Draw(LightingShadowShader, GL_TRIANGLES);
		// }

		// LightingShadowShader.setInt("hasNormalMap",0);

		// // Drawing debug normals gizmos
		
		// // ExplosionShader.UseShaderProgram();
		// // ourModel->Draw(ExplosionShader, GL_TRIANGLES);

		// // Draw Asteriods
		// RenderAsteriods(asteriodModel,&InstanceShader);



		// // Drawing lightsource cubes and highlight

		
		// glBindVertexArray(VAO);

		// LightingShadowShader.UseShaderProgram();
		
	    // // Enable writing to stencil buffer

		// glStencilFunc(GL_ALWAYS,1,0xFF);
		// glStencilMask(0xFF);

		// // Rendering cubes

		// // LightingShadowShader.setInt("material.texture_diffuse1", 0);
		// // glActiveTexture(GL_TEXTURE0);
	    // // glBindTexture(GL_TEXTURE_2D, woodTexture.id);

		// // // LightingShadowShader.setInt("material.texture_specular1", 1);
		// // // glActiveTexture(GL_TEXTURE1);
	    // // // glBindTexture(GL_TEXTURE_2D, woodTexture.id);
		// // glActiveTexture(GL_TEXTURE1);
	    // // glBindTexture(GL_TEXTURE_2D, 0);
		// // for(unsigned int i = 0 ; i<4 ; i++)
		// // {
		// // 	glm::mat4 _model = glm::mat4(1.0f);
		// // 	_model = glm::translate(_model,pointLightPositions[i]);
        // // 	LightingShadowShader.setTransformation("mat_Model",_model);
		// // 	glDrawArrays(GL_TRIANGLES,0,36);
		// // }

		// // glm::mat4 _model = glm::mat4(1.0f);
		// // _model = glm::translate(_model,glm::vec3(0.0f,-2.0f,0.0f));
		// // _model = glm::scale(_model,glm::vec3(30.0f,0.1f,30.0f));
		// // LightingShadowShader.setTransformation("mat_Model", _model);
		// // glDrawArrays(GL_TRIANGLES, 0, 36);

		// // // _model = glm::mat4(1.0f);
		// // // _model = glm::translate(_model, glm::vec3(0.0f, 5.0f, 0.0f));
		// // // _model = glm::scale(_model, glm::vec3(30.0f, 0.1f, 30.0f));
		// // // LightingShadowShader.setTransformation("mat_Model", _model);
		// // // glDrawArrays(GL_TRIANGLES, 0, 36);

		// // glBindTexture(GL_TEXTURE_2D,0);

		// // drawing Transparent objects
        // // glStencilMask(0x00);
		// // ImageShader.UseShaderProgram();
        
		// // for(unsigned int i=0; i < 10 ; i++)
		// // {
		// // 	CubesGameObject[i]->GetComponent<Model>()->Draw(ImageShader,GL_TRIANGLES);
		// // }
        
		// // Disable writing to stencil buffer and just reading its values

		// glStencilFunc(GL_NOTEQUAL,1,0xFF);
		// glStencilMask(0x00);
		// //glDisable(GL_DEPTH_TEST);
		// glDepthMask(GL_ALWAYS);

		// //Drawing highlight cubes
		// // glBindVertexArray(lightVAO);

		// // HighlightShader.UseShaderProgram();

		// // for(unsigned int i = 0 ; i<4 ; i++)
		// // {
		// // 	glm::mat4 _model = glm::mat4(1.0f);
		// // 	_model = glm::translate(_model,pointLightPositions[i]);
		// // 	_model = glm::scale(_model,glm::vec3(1.1f,1.1f,1.1f));
        // // 	HighlightShader.setTransformation("mat_Model",_model);
		// // 	glDrawArrays(GL_TRIANGLES,0,36);
		// // }
		// glEnable(GL_DEPTH_TEST);

		// // RenderSkybox 
		
		// CubeMapShader.UseShaderProgram();
		// glm::mat4 view_nt = glm::mat4(glm::mat3(MainCamera.GetViewMatrix()));  
		// CubeMapShader.setTransformation("mat_View_nt",view_nt);
		// glBindVertexArray(skyboxVAO);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap.id);
        // glDrawArrays(GL_TRIANGLES,0,36);

		// glStencilMask(0xFF);
		// glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// glEnable(GL_DEPTH_TEST);

		// // Physics related //update

		// rb->fixedUpdate(deltaTime);

		// // Late update

		// // Multisampling 
		// glBindFramebuffer(GL_READ_FRAMEBUFFER,msbo);
		// glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);

		// glBlitFramebuffer(0,0,SRC_WIDTH,SRC_HEIGHT,0,0,SRC_WIDTH,SRC_HEIGHT,GL_COLOR_BUFFER_BIT,GL_NEAREST);
		// glDisable(GL_DEPTH_TEST);

		// // Brightness processing
        // glBindFramebuffer(GL_FRAMEBUFFER,fbo);
		// BrightShader.UseShaderProgram();
		// BrightShader.setInt("screenTexture",0);
        
		// // unsigned int attachMents[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		// // glDrawBuffers(2,attachMents);

		// glBindVertexArray(quadVAO);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D,colorBuffer.id);
        // glDrawArrays(GL_TRIANGLES,0,6);

        // glBindVertexArray(0);
		// glBindFramebuffer(GL_FRAMEBUFFER,0);

		// // Blurring Brightness
		// bool Horizontal = true, isFirstIteration = true;
		// unsigned int samples = 6;
        // BloomShader.UseShaderProgram();
		// glBindVertexArray(quadVAO);

		// for(unsigned int i=0; i<samples; i++)
		// {
		// 	glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBO[Horizontal]);
		// 	BloomShader.setInt("horizontal",Horizontal);

		// 	glActiveTexture(GL_TEXTURE0);
		// 	glBindTexture(GL_TEXTURE_2D,isFirstIteration ? brightColorBuffer.id : pingpongBuffers[!Horizontal].id);
        //     glDrawArrays(GL_TRIANGLES,0,6);
		// 	Horizontal = !Horizontal;

		// 	isFirstIteration = false;
		// }
		// glBindFramebuffer(GL_FRAMEBUFFER,0);


		// // Post processing

		// glBindFramebuffer(GL_FRAMEBUFFER,0);
		// glDisable(GL_DEPTH_TEST);
		// glClearColor(0.1f,0.1f,0.1f,0.1f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// // unsigned int attachMents[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		// // glDrawBuffers(2,attachMents);

        // PostShader.UseShaderProgram();
		// PostShader.setFloat("exposure",1.0f);
		// PostShader.setInt("scene",0);
		// PostShader.setInt("bloomBlur",1);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D,colorBuffer.id);
		// glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D,pingpongBuffers[0].id);
		// glActiveTexture(GL_TEXTURE0);

		// glBindVertexArray(quadVAO);
        // glDrawArrays(GL_TRIANGLES,0,6);


		//Check for all events and swaps buffers

		h_Panel.OnImGuiRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Ending IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Call OnDestroy/End functions here
	stopPhysicsThread();

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void process_inputs(GLFWwindow* window)
{
	cameraSpeed = 3.5f* deltaTime;
	if (KeyBoard::KeyWentDown(GLFW_KEY_1))
	{
		glfwSetWindowShouldClose(window, 1);
	}

	if(KeyBoard::KeyWentDown(GLFW_KEY_UP))
	{
		Arrow_vertical_Input += 0.01f;
		if(Arrow_vertical_Input >= 1.0f) Arrow_vertical_Input = 1.0f;
	}
	if(KeyBoard::KeyWentDown(GLFW_KEY_DOWN))
	{
		Arrow_vertical_Input -= 0.01f;
		if(Arrow_vertical_Input <= 0.0f) Arrow_vertical_Input = 0.0f;
	}
	if(KeyBoard::Key(GLFW_KEY_W))
	{
		MainCamera.ProcessWASD(CAMERA_FORWARD,deltaTime);
	}
	if(KeyBoard::Key(GLFW_KEY_S))
	{
		MainCamera.ProcessWASD(CAMERA_BACKWARD,deltaTime);
	}
	if(KeyBoard::Key(GLFW_KEY_A))
	{
		MainCamera.ProcessWASD(CAMERA_LEFT,deltaTime);
	}
	if(KeyBoard::Key(GLFW_KEY_D))
	{
		MainCamera.ProcessWASD(CAMERA_RIGHT,deltaTime);
	}
	if(Mouse::MouseButton(GLFW_MOUSE_BUTTON_1))
	{
		MainCamera.ProcessMouse(float(Mouse::getMouseDX()),float(Mouse::getMouseDY()));
	}
	MainCamera.SetZoom(float(Mouse::getMouseWheelY()));
	// Extract current scale from Scale matrix
	glm::vec3 currentScale = glm::vec3(
		glm::length(glm::vec3(Scale[0])), // X scaleth
		glm::length(glm::vec3(Scale[1])), // Y scale
		glm::length(glm::vec3(Scale[2]))  // Z scale
	);

	// Modify scale based on mouse wheel input
	float scaleFactor = 1.0f + (0.01f * float(Mouse::getMouseWheelY())); // Scaling factor
	currentScale *= scaleFactor;								  // Uniform scaling

	// Apply new scale (create a fresh scale matrix)
	Scale = glm::scale(glm::mat4(1.0f), currentScale);
}
void RenderAsteriods(Model* m,Shader* s)
{
    //s->UseShaderProgram();
	//m->DrawInstanced(*s,GL_TRIANGLES,asteriodInstances);
}

void RenderScene(Shader* shader,std::vector<Model*> models,int cubeVAO)
{
	shader->UseShaderProgram();
    for(auto& model : models)
	{
		model->Draw(*shader,GL_TRIANGLES);
	}

	// glBindVertexArray(cubeVAO);

	// for (unsigned int i = 0; i < 4; i++)
	// {
	// 	glm::mat4 _model = glm::mat4(1.0f);
	// 	_model = glm::translate(_model, pointLightPositions[i]);
	// 	shader->setTransformation("mat_Model", _model);
	// 	glDrawArrays(GL_TRIANGLES, 0, 36);
	// }

	// glm::mat4 _model = glm::mat4(1.0f);
	// _model = glm::translate(_model, glm::vec3(0.0f, -2.0f, 0.0f));
	// _model = glm::scale(_model, glm::vec3(30.0f, 0.1f, 30.0f));
	// shader->setTransformation("mat_Model", _model);
	// glDrawArrays(GL_TRIANGLES, 0, 36);

	// _model = glm::mat4(1.0f);
	// _model = glm::translate(_model, glm::vec3(0.0f, 5.0f, 0.0f));
	// _model = glm::scale(_model, glm::vec3(30.0f, 0.1f, 30.0f));
	// shader->setTransformation("mat_Model", _model);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
}

void SetViewAndProjectionForAllShaders(unsigned int uboIndex)
{
	// for(auto& shader : shaders)
	// {
	// 	shader->UseShaderProgram();
    //     shader->setTransformation("mat_View",MainCamera.GetViewMatrix());
	// 	shader->setTransformation("mat_Projection",MainCamera.GetProjectionMatrix());
	// }

	// Setting view mattrix in uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER,uboIndex);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(MainCamera.GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(MainCamera.GetProjectionViewMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER,0);
}


