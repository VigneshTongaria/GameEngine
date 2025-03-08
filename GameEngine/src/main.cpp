#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include "Shader.h"
#include <stb/stb_image.h>
#include "io/KeyBoard.h"
#include "io/Mouse.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "physics/rigidbody.hpp"
#include "managers/ResourcesManager.hpp"
#include "data/GeometryData.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);
void Mat_Calculations();
std::string loadShaderSRC(const char* filename);
float Arrow_vertical_Input = 0.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
glm::mat4 mouseScroll = glm::mat4(1.0f);
glm::mat4 Scale = glm::mat4(1.0f);
glm::mat4 m_Model = glm::mat4(1.0f);
glm::mat4 View = glm::mat4(1.0f);
glm::mat4 Projection = glm::mat4(1.0f); 
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float cameraSpeed = 0.1f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera MainCamera;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
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
	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window,KeyBoard::KeyCallback);
	glfwSetMouseButtonCallback(window,Mouse::MouseButtonCallback);
	glfwSetCursorPosCallback(window,Mouse::CursorButtonCallback);
	glfwSetScrollCallback(window,Mouse::MouseScrollCallback);

	//LoadTextures
	// unsigned int texture_1 = loadTexture("Assets/Images/container2.png");
	// unsigned int texture_2 = loadTexture("Assets/Images/container2_specular.png");
	// unsigned int texture_3 = loadTexture("Assets/Images/container2_emissive.jpg");
	

	//Run_Shaders();
	//Vertex shader
	Shader LightingShader("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	Shader LightnigSourceShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_lightSource.glsl");
	Shader HighlightShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_highlight.glsl");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	}; 
	
	unsigned int indices[] = 
	{
		0,1,3, //first traingle
		1,2,3
	};
	//Initializng buffers
	unsigned int VBO, VBO_1,VAO,VAO_1,EBO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO_1);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_1);
	glGenBuffers(1,&EBO);
    
	//Binding buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //vertex attrbitue pointer assigning all types of data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1,3,GL_FLOAT,false,8 * sizeof(float),(void*)(3*(sizeof(float))));
	glVertexAttribPointer(2,2,GL_FLOAT,false,8 * sizeof(float),(void*)(6*(sizeof(float))));
	//enabling all the attribute array
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//Assigning textures ID'S
	LightingShader.UseShaderProgram();
	// LightingShader.setInt("Texture1",0);
	// LightingShader.setInt("Texture2",1);

	//Lighting VAO

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	/*glm::mat4 Rot_0 = glm::mat4(1.0);
	Rot_0 = glm::rotate(Rot_0, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	LightnigShader_1.UseShaderProgram();

	LightnigShader_1.setTransformation("mat_Rotation", Rot_0);*/
	//setting LightingShader values
	m_Model = glm::rotate(m_Model,glm::radians(-45.0f), glm::vec3(1.0f,0.0f,0.0f));

	Projection = glm::perspective(glm::radians(60.0f),800.0f/600.0f,0.1f,100.0f);
	LightingShader.setTransformation("mat_Projection",Projection);
	LightingShader.setFloat("material.shininess",32.0f);
    
	GameObject gameObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));

    //	Adding gameObject components for  our model
	gameObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/backpack/backpack.obj");
	Model* ourModel = gameObject.GetComponent<Model>();

	gameObject.AddComponent<Rigidbody>(1.0f,glm::vec3(0.0f), glm::vec3(0.0f,0.0f,0.0f));
	Rigidbody* rb = gameObject.GetComponent<Rigidbody>();

	// Texture for cubes
	std::vector<Texture> newTexture;
	newTexture.push_back( ResourcesManager::loadTexture("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/grass.png",TEXTURE_TYPE::DIFFUSE));

	// Gameobjects for cubes
	GameObject** CubesGameObject = new GameObject*[10];
    for(unsigned int i = 0; i < 10; i++)
	{
        CubesGameObject[i] = new GameObject(cubePositions[i]);
		CubesGameObject[i]->AddComponent<Model>(DEFAULT_MODEL::CUBE,newTexture);
	}
    

	LightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	LightingShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	LightingShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	// point light 1
	LightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	LightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setFloat("pointLights[0].constant", 1.0f);
	LightingShader.setFloat("pointLights[0].linear", 0.09f);
	LightingShader.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	LightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	LightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setFloat("pointLights[1].constant", 1.0f);
	LightingShader.setFloat("pointLights[1].linear", 0.09f);
	LightingShader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	LightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	LightingShader.setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShader.setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setFloat("pointLights[2].constant", 1.0f);
	LightingShader.setFloat("pointLights[2].linear", 0.09f);
	LightingShader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	LightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	LightingShader.setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShader.setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setFloat("pointLights[3].constant", 1.0f);
	LightingShader.setFloat("pointLights[3].linear", 0.09f);
	LightingShader.setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight

	LightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	LightingShader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setFloat("spotLight.constant", 1.0f);
	LightingShader.setFloat("spotLight.linear", 0.09f);
	LightingShader.setFloat("spotLight.quadratic", 0.032f);
	LightingShader.setFloat("spotLight.cosTheta", glm::cos(glm::radians(12.5f)));
	LightingShader.setFloat("spotLight.cosThetaOuter", glm::cos(glm::radians(15.0f)));
    
	// Cube Shaders 
	LightnigSourceShader.UseShaderProgram();
	LightnigSourceShader.setTransformation("mat_Projection",Projection);

	HighlightShader.UseShaderProgram();
	HighlightShader.setTransformation("mat_Projection",Projection);

	//camera

	// Call all start functions here
    lastFrame = float(glfwGetTime());
	rb->Start();

	while (!glfwWindowShouldClose(window))
	{
		//calculate deltaTime
		float currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		//process inputs
		process_inputs(window);

		//rendering
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP,GL_REPLACE,GL_REPLACE);

		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// Disable writing to stencil buffer
		glStencilMask(0x00);

		//draw shapes
		LightingShader.UseShaderProgram();

		float time = static_cast<float>(glfwGetTime());
		View = MainCamera.GetViewMatrix();

		glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		
		//  LightPositions[0].x = 1.0f*glm::sin(glm::radians(time*10.0f));
		//  LightPositions[0].z = 1.0f*glm::cos(glm::radians(time*10.0f));
		LightingShader.setVec3("spotLight.position", MainCamera.GetCameraPos());
		LightingShader.setVec3("spotLight.direction", MainCamera.GetCameraFront());

        LightingShader.setTransformation("mat_View",View);
		LightingShader.setVec3("viewPos",MainCamera.GetCameraPos());
		
		// for(unsigned int i = 0 ; i<10 ; i++)
		// {
		// 	glm::mat4 _model = glm::mat4(1.0f);
		// 	_model = glm::translate(_model,cubePositions[i]);
        // 	LightingShader.setTransformation("mat_Model",_model);
		// 	glDrawArrays(GL_TRIANGLES,0,36);
		// }
		// writing stencil on models

		ourModel->Draw(LightingShader);

		// drawing cubes
        
		for(unsigned int i=0; i < 10 ; i++)
		{
			CubesGameObject[i]->GetComponent<Model>()->Draw(LightingShader);
		}
		

		glBindVertexArray(lightVAO);

		LightnigSourceShader.UseShaderProgram();
		LightnigSourceShader.setTransformation("mat_View",View);
		
	    // Enable writing to stencil buffer

		glStencilFunc(GL_ALWAYS,1,0xFF);
		glStencilMask(0xFF);

		for(unsigned int i = 0 ; i<4 ; i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model,pointLightPositions[i]);
        	LightnigSourceShader.setTransformation("mat_Model",_model);
			glDrawArrays(GL_TRIANGLES,0,36);
		}
        
		// Disable writing to stencil buffer and just reading its values

		glStencilFunc(GL_NOTEQUAL,1,0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		//Drawing highlight cubes

		HighlightShader.UseShaderProgram();
		HighlightShader.setTransformation("mat_View",View);
		for(unsigned int i = 0 ; i<4 ; i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model,pointLightPositions[i]);
			_model = glm::scale(_model,glm::vec3(1.1f,1.1f,1.1f));
        	HighlightShader.setTransformation("mat_Model",_model);
			glDrawArrays(GL_TRIANGLES,0,36);
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// Physics related //update

		rb->Update(deltaTime);


		//Check for all events and swaps buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

void Mat_Calculations()
{
	glm::vec4 vec(1.0f, 3.0f, 0.0f, 1.0f);
	glm::mat4 mat_T = glm::mat4(1.0f);
	glm::mat4 trans = glm::translate(mat_T, glm::vec3(0.0f, 6.4f, 3.0f));
	glm::mat4 rot = glm::rotate(mat_T, 60.0f, glm::vec3(5.0f, 0.0f, 60.0f));
	glm::mat4 scale = glm::scale(mat_T, glm::vec3(0.5f, 0.4f, 0.4f));

	vec = scale*rot*trans * vec;
	std::cout << vec.x << " "<< vec.y << " "<<vec.z << std::endl;
}


std::string loadShaderSRC(const char* filename)
{
    std::ifstream file;
	std::stringstream buf;
	std::string ret= "";

	file.open(filename);
	if(file.is_open())
	{
        buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
       std::cout<<"Could not open "<<filename<< std::endl;

	}
	file.close();
    return ret;
}


