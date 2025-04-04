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
void SetViewAndProjectionForAllShaders(unsigned int uboIndex);
void RenderAsteriods(Model* m,Shader* s);
std::string loadShaderSRC(const char* filename);
float Arrow_vertical_Input = 0.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
glm::mat4 mouseScroll = glm::mat4(1.0f);
glm::mat4 Scale = glm::mat4(1.0f);
float cameraSpeed = 0.1f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
unsigned int asteriodInstances = 10.0f;

Camera MainCamera;

// Asteriods mattrices
std::vector<glm::mat4> asteriodModelMats;

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
	Shader ImageShader("Assets/vertex_core.glsl", "Assets/fragment_core_1.glsl");
	Shader HighlightShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_highlight.glsl");
	Shader PostShader("Assets/vertex_unlit.glsl", "Assets/fragment_post.glsl");
	Shader CubeMapShader("Assets/vertex_cubeMap.glsl", "Assets/fragment_cubeMap.glsl");
	Shader ExplosionShader("Assets/GeometryShaders/Vertex_unlit.glsl", 
		"Assets/fragment_core_highlight.glsl","Assets/GeometryShaders/Geometry_normals.glsl");
	Shader InstanceShader("Assets/vertex_Instance.glsl", "Assets/GeometryShaders/fragment_unlit.glsl");

	shaders.push_back(&LightingShader);
	shaders.push_back(&LightnigSourceShader);
	shaders.push_back(&ImageShader);
	shaders.push_back(&HighlightShader);
	shaders.push_back(&PostShader);
	shaders.push_back(&CubeMapShader);
	shaders.push_back(&ExplosionShader);
	shaders.push_back(&InstanceShader);

	// Adding uniform buffer index
	unsigned int uniformVertexCoreIndex = glGetUniformBlockIndex(LightingShader.m_ID,"Matrices");
	unsigned int uniformVertexSkyboxIndex = glGetUniformBlockIndex(CubeMapShader.m_ID,"Matrices");
	unsigned int uniformVertexLightingSourceIndex = glGetUniformBlockIndex(LightnigSourceShader.m_ID,"Matrices");
	unsigned int uniformVertexSimplendex = glGetUniformBlockIndex(ExplosionShader.m_ID,"Matrices");
	unsigned int uniformVertexInstanceIndex = glGetUniformBlockIndex(InstanceShader.m_ID,"Matrices");

	glUniformBlockBinding(LightingShader.m_ID,uniformVertexCoreIndex,0);
	glUniformBlockBinding(CubeMapShader.m_ID,uniformVertexSkyboxIndex,0);
	glUniformBlockBinding(LightnigSourceShader.m_ID,uniformVertexLightingSourceIndex,0);
	glUniformBlockBinding(ExplosionShader.m_ID,uniformVertexSimplendex,0);
	glUniformBlockBinding(InstanceShader.m_ID,uniformVertexInstanceIndex,0);

    // Vertices data not used now added geometrydata class but still used in VBO 
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
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
	
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
	
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
	
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
	
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
	
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
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

	// Vertex data for quad

	unsigned int quadVBO,quadVAO;
	glGenVertexArrays(1,&quadVAO);
	glGenBuffers(1,&quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


	//Lighting VAO

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Skybox VAO

	unsigned int skyboxVAO,skyboxVBO;
    
	glGenVertexArrays(1,&skyboxVAO);
	glGenBuffers(1,&skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER,skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	// Generating Frame buffers

	unsigned int fbo;
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	Texture colorBuffer = ResourcesManager::loadTexture(GL_RGB,800,600);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorBuffer.id,0);

	// Generating render buffer objects
	unsigned int rbo;
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,800,600);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  std::cout<<"Frame buffers not generated"<<std::endl;
	
	glBindRenderbuffer(GL_RENDERBUFFER,0);

	// Generating uniform buffers
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	//setting LightingShader values
	LightingShader.setFloat("material.shininess",32.0f);
    
	GameObject gameObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));

    //	Adding gameObject components for  our model
	gameObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/backpack/backpack.obj");
	Model* ourModel = gameObject.GetComponent<Model>();

	gameObject.AddComponent<Rigidbody>(1.0f,glm::vec3(0.0f), glm::vec3(0.0f,0.0f,0.0f));
	Rigidbody* rb = gameObject.GetComponent<Rigidbody>();

	// Adding asteriod belt GameObject
	GameObject asteriodObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
	asteriodObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/rock/rock.obj",asteriodInstances);

	Model* asteriodModel = asteriodObject.GetComponent<Model>();

	// Texture for cubes
	std::vector<Texture> newTexture;
	newTexture.push_back( ResourcesManager::loadTexture("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/blending_transparent_window.png",TEXTURE_TYPE::DIFFUSE));

	// Gameobjects for cubes
	GameObject** CubesGameObject = new GameObject*[10];
    for(unsigned int i = 0; i < 10; i++)
	{
        CubesGameObject[i] = new GameObject(cubePositions[i]);
		CubesGameObject[i]->AddComponent<Model>(DEFAULT_MODEL::CUBE,newTexture);
	}

	// Loading cubeMap

	CubeMap cubeMap = ResourcesManager::loadCubeMap(cubeFaces);
	CubeMapShader.UseShaderProgram();
	CubeMapShader.setInt("skybox",0);
    
	LightingShader.UseShaderProgram();

	// Setting reflection probe
	LightingShader.setInt("reflection",0);

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

	HighlightShader.UseShaderProgram();

	ImageShader.UseShaderProgram();

	//camera

	// Set projection mattrix in the uniform buffer

	glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(MainCamera.GetProjectionMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	// Call all start functions here
    lastFrame = float(glfwGetTime());
	rb->Start();
	ResourcesManager::VerticesCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		//calculate deltaTime
		float currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		//process inputs
		process_inputs(window);
		SetViewAndProjectionForAllShaders(uboMatrices);

		//rendering
		ResourcesManager::VerticesCount = 0;

		// Binding framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER,fbo);

		// All tests
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		//glEnable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glStencilOp(GL_KEEP,GL_REPLACE,GL_REPLACE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// Disable writing to stencil buffer
		glStencilMask(0x00);
 
		//draw shapes
		LightingShader.UseShaderProgram();

		float time = static_cast<float>(glfwGetTime());

		glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		
		//  LightPositions[0].x = 1.0f*glm::sin(glm::radians(time*10.0f));
		//  LightPositions[0].z = 1.0f*glm::cos(glm::radians(time*10.0f));
		LightingShader.setVec3("spotLight.position", MainCamera.GetCameraPos());
		LightingShader.setVec3("spotLight.direction", MainCamera.GetCameraFront());

		LightingShader.setVec3("viewPos",MainCamera.GetCameraPos());
		
		// for(unsigned int i = 0 ; i<10 ; i++)
		// {
		// 	glm::mat4 _model = glm::mat4(1.0f);
		// 	_model = glm::translate(_model,cubePositions[i]);
        // 	LightingShader.setTransformation("mat_Model",_model);
		// 	glDrawArrays(GL_TRIANGLES,0,36);
		// }
		// writing stencil on models

		ourModel->Draw(LightingShader,GL_TRIANGLES);

		// Drawing debug normals gizmos
		
		ExplosionShader.UseShaderProgram();
		ourModel->Draw(ExplosionShader, GL_TRIANGLES);

		// Draw Asteriods
		RenderAsteriods(asteriodModel,&InstanceShader);



		// Drawing lightsource cubes and highlight

		
		glBindVertexArray(lightVAO);

		LightnigSourceShader.UseShaderProgram();
		
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

		// drawing Transparent objects
        glStencilMask(0x00);
		ImageShader.UseShaderProgram();
        
		for(unsigned int i=0; i < 10 ; i++)
		{
			CubesGameObject[i]->GetComponent<Model>()->Draw(ImageShader,GL_TRIANGLES);
		}
        
		// Disable writing to stencil buffer and just reading its values

		glStencilFunc(GL_NOTEQUAL,1,0xFF);
		glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_ALWAYS);

		//Drawing highlight cubes
		glBindVertexArray(lightVAO);

		HighlightShader.UseShaderProgram();

		for(unsigned int i = 0 ; i<4 ; i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model,pointLightPositions[i]);
			_model = glm::scale(_model,glm::vec3(1.1f,1.1f,1.1f));
        	HighlightShader.setTransformation("mat_Model",_model);
			glDrawArrays(GL_TRIANGLES,0,36);
		}

		// RenderSkybox 
		glEnable(GL_DEPTH_TEST);
		
		CubeMapShader.UseShaderProgram();
		glm::mat4 view_nt = glm::mat4(glm::mat3(MainCamera.GetViewMatrix()));  
		CubeMapShader.setTransformation("mat_View_nt",view_nt);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap.id);
        glDrawArrays(GL_TRIANGLES,0,36);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// Physics related //update

		rb->Update(deltaTime);

		// Late update

		// Post processing

		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.1f,0.1f,0.1f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

        PostShader.UseShaderProgram();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D,colorBuffer.id);
        glDrawArrays(GL_TRIANGLES,0,6);


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

void RenderAsteriods(Model* m,Shader* s)
{
    s->UseShaderProgram();
	m->DrawInstanced(*s,GL_TRIANGLES,asteriodInstances);
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
	glBindBuffer(GL_UNIFORM_BUFFER,0);
}


