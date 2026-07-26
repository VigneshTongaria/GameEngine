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

	//Model cityModel = Model("C:/Github/GameEngine/GameEngine/GameEngine/Assets/resources/Girl/a_contortionist_dancer.glb");

	//Model cityModel = Model("C:/Github/GameEngine/GameEngine/GameEngine/Assets/resources/the-armoury/source/armory/model.obj");

	//cityModel.addModelToScene(&scene);

	MainCamera.SetCameraPos(glm::vec3(0.0f));

	Model underWearGirl = Model("C:/Github/GameEngine/GameEngine/GameEngine/Assets/resources/Girl/in_underwear.glb");

	underWearGirl.addModelToScene(&scene);

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


