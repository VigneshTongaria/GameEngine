#include<iostream>
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
void RenderScene(Shader* shader,std::vector<Model*> models,int cubeVAO);
void RenderAsteriods(Model* m,Shader* s);
std::string loadShaderSRC(const char* filename);
float Arrow_vertical_Input = 0.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
glm::mat4 mouseScroll = glm::mat4(1.0f);
glm::mat4 Scale = glm::mat4(1.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
}; 
float cameraSpeed = 0.1f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
unsigned int NR_POINT_LIGHTS = 4;
unsigned int asteriodInstances = 1000;
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

	//LoadTextures
	// unsigned int texture_1 = loadTexture("Assets/Images/container2.png");
	// unsigned int texture_2 = loadTexture("Assets/Images/container2_specular.png");
	// unsigned int texture_3 = loadTexture("Assets/Images/container2_emissive.jpg");
	
	// Setting up camera
	MainCamera.height = SRC_HEIGHT;
	MainCamera.width = SRC_WIDTH;

	//Run_Shaders();
	//Vertex shader
	Shader LightingShader("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	Shader LightnigSourceShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_lightSource.glsl");
	Shader ImageShader("Assets/vertex_core.glsl", "Assets/fragment_core_1.glsl");
	Shader HighlightShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_highlight.glsl");
	Shader PostShader("Assets/vertex_unlit.glsl", "Assets/fragment_Blending.glsl");
	Shader CubeMapShader("Assets/vertex_cubeMap.glsl", "Assets/fragment_cubeMap.glsl");
	Shader ExplosionShader("Assets/GeometryShaders/Vertex_unlit.glsl", 
		"Assets/fragment_core_highlight.glsl","Assets/GeometryShaders/Geometry_normals.glsl");
	Shader InstanceShader("Assets/vertex_Instance.glsl", "Assets/GeometryShaders/fragment_unlit.glsl");
	Shader DepthMapShader("Assets/vertex_depthMap.glsl", "Assets/fragment_depthMap.glsl");
	Shader LightingShadowShader("Assets/vertex_core_shadows.glsl", "Assets/fragment_core_shadows.glsl");
	Shader PointLightingShadowShader("Assets/vertex_pointLight.glsl","Assets/GeometryShaders/fragment_PointLightMap.glsl",
		"Assets/GeometryShaders/Geometry_PointLight.glsl");
	Shader BrightShader("Assets/vertex_unlit.glsl", "Assets/fragment_brightness.glsl");
	Shader BloomShader("Assets/vertex_unlit.glsl", "Assets/fragment_bloom.glsl");

	shaders.push_back(&LightingShader);
	shaders.push_back(&LightnigSourceShader);
	shaders.push_back(&ImageShader);
	shaders.push_back(&HighlightShader);
	shaders.push_back(&PostShader);
	shaders.push_back(&CubeMapShader);
	shaders.push_back(&ExplosionShader);
	shaders.push_back(&InstanceShader);
	shaders.push_back(&LightingShadowShader);

	// Adding uniform buffer index
	unsigned int uniformVertexCoreIndex = glGetUniformBlockIndex(LightingShader.m_ID,"Matrices");
	unsigned int uniformVertexSkyboxIndex = glGetUniformBlockIndex(CubeMapShader.m_ID,"Matrices");
	unsigned int uniformVertexLightingSourceIndex = glGetUniformBlockIndex(LightnigSourceShader.m_ID,"Matrices");
	unsigned int uniformVertexSimplendex = glGetUniformBlockIndex(ExplosionShader.m_ID,"Matrices");
	unsigned int uniformVertexInstanceIndex = glGetUniformBlockIndex(InstanceShader.m_ID,"Matrices");
	unsigned int uniformVertexCoreShadowsIndex = glGetUniformBlockIndex(LightingShadowShader.m_ID,"Matrices");

	glUniformBlockBinding(LightingShader.m_ID,uniformVertexCoreIndex,0);
	glUniformBlockBinding(CubeMapShader.m_ID,uniformVertexSkyboxIndex,0);
	glUniformBlockBinding(LightnigSourceShader.m_ID,uniformVertexLightingSourceIndex,0);
	glUniformBlockBinding(ExplosionShader.m_ID,uniformVertexSimplendex,0);
	glUniformBlockBinding(InstanceShader.m_ID,uniformVertexInstanceIndex,0);
	glUniformBlockBinding(LightingShadowShader.m_ID,uniformVertexCoreShadowsIndex,0);

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

	// Multisampling buffers
	unsigned int msbo;
	glGenFramebuffers(1,&msbo);
	glBindFramebuffer(GL_FRAMEBUFFER,msbo);

	unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB,SRC_WIDTH, SRC_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,textureColorBufferMultiSampled,0);

	// Multisample Renderbuffers
	unsigned int mrbo;
	glGenRenderbuffers(1,&mrbo);
	glBindRenderbuffer(GL_RENDERBUFFER,mrbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,4,GL_DEPTH24_STENCIL8,SRC_WIDTH, SRC_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,mrbo);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  std::cout<<"Frame buffers not generated"<<std::endl;
	
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// Post processing Frame buffer
	unsigned int fbo;
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	Texture colorBuffer = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	Texture brightColorBuffer = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorBuffer.id,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,brightColorBuffer.id,0);

	// Blur frameBuffer
	unsigned int pingpongFBO[2];
	glGenFramebuffers(2, pingpongFBO);
	Texture pingpongBuffers[2];
	pingpongBuffers[0] = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);
	pingpongBuffers[1] = ResourcesManager::loadTexture(GL_RGBA,GL_RGBA16F,SRC_WIDTH, SRC_HEIGHT);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i].id, 0);
	}

	// Post processing renderBuffers
	unsigned int rbo;
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,SRC_WIDTH, SRC_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  std::cout<<"Frame buffers not generated"<<std::endl;
	
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// Generating directional light depth map buffers

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthMap,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Generating point light cubemaps depth buffers

	std::vector<unsigned int> pointLightDepthMap;
	std::vector<unsigned int> pointLightDepthMapBuffers;

	for(unsigned int i = 0; i<NR_POINT_LIGHTS; i++)
	{
        unsigned int depthCubeMap;
		glGenTextures(1, &depthCubeMap);
		unsigned int depthBuffer;
		glGenFramebuffers(1,&depthBuffer);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		for(unsigned int j=0 ; j<6 ; j++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER,depthBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap,0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		pointLightDepthMap.push_back(depthCubeMap);
		pointLightDepthMapBuffers.push_back(depthBuffer);
	}

	// Generating uniform buffers
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
	
    
	GameObject gameObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(10.0f,30.0f,50.0f),glm::vec3(1.0f,1.0f,1.0f));

    //	Adding gameObject components for  our model
	// gameObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/backpack/backpack.obj");
	// Model* ourModel = gameObject.GetComponent<Model>();

	// SceneModels.push_back(ourModel);

	gameObject.AddComponent<Rigidbody>(1.0f,glm::vec3(0.0f), glm::vec3(0.0f,0.0f,0.0f));
	Rigidbody* rb = gameObject.GetComponent<Rigidbody>();

	// Adding entire city
	GameObject cityObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-90.0f,0.0f,0.0f),glm::vec3(0.3f,0.3f,0.3f));
	cityObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/City/City.glb");
	Model* cityModel = cityObject.GetComponent<Model>();

	SceneModels.push_back(cityModel);

	// Adding asteriod belt GameObject
	GameObject asteriodObject(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(30.0f,50.0f,10.0f),glm::vec3(1.0f,1.0f,1.0f));
	asteriodObject.AddComponent<Model>("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/rock/rock.obj",asteriodInstances);

	Model* asteriodModel = asteriodObject.GetComponent<Model>();

	// Texture for cubes
	std::vector<Texture> newTexture;
	newTexture.push_back( ResourcesManager::loadTexture("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/blending_transparent_window.png",TEXTURE_TYPE::DIFFUSE));

	// Gameobjects for cubes
	// GameObject** CubesGameObject = new GameObject*[10];
    // for(unsigned int i = 0; i < 10; i++)
	// {
    //     CubesGameObject[i] = new GameObject(cubePositions[i]);
	// 	CubesGameObject[i]->AddComponent<Model>(DEFAULT_MODEL::CUBE,newTexture);
	// }

	// Adding light projection and view matrices
	float near_plane = 1.0f,far_plane = 170.5f;
	glm::vec3 DirectionalLightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::mat4 lightView = glm::lookAt(-10.0f* DirectionalLightDir,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 lightProj = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane, far_plane);
	glm::mat4 lightSpaceMatrix =  lightProj *lightView;

	// points light view and projection matrix

	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<std::vector<glm::mat4>> pointLightsViewProjection;

	for(unsigned int i=0;i<NR_POINT_LIGHTS;i++)
	{
		glm::vec3 lightPos = pointLightPositions[i];
		std::vector<glm::mat4> newLightView;
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		newLightView.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		// for (int i = 0; i < 6; ++i) {
		// 	std::cout << "Matrix " << i << ":\n" << glm::to_string(newLightView[i]) << std::endl;
		// }
		pointLightsViewProjection.push_back(newLightView);
	}

	// Loading cubeMap

	CubeMap cubeMap = ResourcesManager::loadCubeMap(cubeFaces);
	CubeMapShader.UseShaderProgram();
	CubeMapShader.setInt("skybox",0);
    
	LightingShader.UseShaderProgram();

	// Loading miscallaneous textures
	Texture woodTexture = ResourcesManager::loadTexture("C:/Users/vigne/GithubRepos/GameEngine/GameEngine/Assets/resources/ExtraTextures/Wood_base.jpg",TEXTURE_TYPE::DIFFUSE);

	// Setting reflection probe
	LightingShader.setInt("reflection",0);
	LightingShader.setFloat("material.shininess",32.0f);

	LightingShader.setVec3("dirLight.direction", DirectionalLightDir);
	LightingShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	LightingShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));
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

	LightingShadowShader.UseShaderProgram();
	LightingShadowShader.setFloat("material.shininess",32.0f);
	LightingShadowShader.setInt("reflection",13);

	LightingShadowShader.setVec3("dirLight.direction", DirectionalLightDir);
	LightingShadowShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
	LightingShadowShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShadowShader.setVec3("dirLight.specular", glm::vec3(0.4f, 0.4f, 0.4f));

	LightingShadowShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	LightingShadowShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShadowShader.setVec3("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShadowShader.setVec3("pointLights[0].specular", glm::vec3(0.4f, 0.4f, 0.4f));
	LightingShadowShader.setFloat("pointLights[0].constant", 1.0f);
	LightingShadowShader.setFloat("pointLights[0].linear", 0.09f);
	LightingShadowShader.setFloat("pointLights[0].quadratic", 0.032f);

	LightingShadowShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	LightingShadowShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShadowShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShadowShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShadowShader.setFloat("pointLights[1].constant", 1.0f);
	LightingShadowShader.setFloat("pointLights[1].linear", 0.09f);
	LightingShadowShader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	LightingShadowShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	LightingShadowShader.setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShadowShader.setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShadowShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShadowShader.setFloat("pointLights[2].constant", 1.0f);
	LightingShadowShader.setFloat("pointLights[2].linear", 0.09f);
	LightingShadowShader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	LightingShadowShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	LightingShadowShader.setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	LightingShadowShader.setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	LightingShadowShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	LightingShadowShader.setFloat("pointLights[3].constant", 1.0f);
	LightingShadowShader.setFloat("pointLights[3].linear", 0.09f);
	LightingShadowShader.setFloat("pointLights[3].quadratic", 0.032f);

	// Setting and binding all the shadow depth maps

	for(unsigned int i=0 ;i<NR_POINT_LIGHTS; i++)
	{
		LightingShadowShader.setInt("pointShadowMap["+std::to_string(i)+"]",5 + i);
	}
	LightingShadowShader.setFloat("far_plane",far);
	LightingShadowShader.setInt("hasNormalMap",0);
	int loc = glGetUniformLocation(LightingShadowShader.m_ID, "hasNormalMap");
    std::cout << "hasNormalMap location: " << loc << std::endl;

	glBindTexture(GL_TEXTURE_2D,depthMap);
	LightingShadowShader.setTransformation("mat_Lightspace",lightSpaceMatrix);
	LightingShadowShader.setInt("shadowMap",10);


	glBindTexture(GL_TEXTURE_2D,0);

	// Depth map shader values set
	DepthMapShader.UseShaderProgram();
	DepthMapShader.setTransformation("lightSpaceMatrix",lightSpaceMatrix);
    
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

		// Rendering scene first for depth Map for directional light
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		RenderScene(&DepthMapShader,SceneModels,lightVAO);
        
		for(unsigned int i=0; i<NR_POINT_LIGHTS; i++)
		{
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); 
			glBindFramebuffer(GL_FRAMEBUFFER,pointLightDepthMapBuffers[i]);
			glClear(GL_DEPTH_BUFFER_BIT);
			PointLightingShadowShader.UseShaderProgram();
			PointLightingShadowShader.setVec3("lightPos",pointLightPositions[i]);
			PointLightingShadowShader.setFloat("far_plane",far);
			for(unsigned int j=0; j<6; j++)
			{
				PointLightingShadowShader.setTransformation("pointLightSpaceView[" + std::to_string(j) + "]",pointLightsViewProjection[i][j]);
			}
			RenderScene(&PointLightingShadowShader,SceneModels,lightVAO);
		}

		// Binding framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER,msbo);

		// All tests
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glStencilOp(GL_KEEP,GL_REPLACE,GL_REPLACE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);

		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// Disable writing to stencil buffer
		//glStencilMask(0x00);

 
		//draw shapes
		LightingShadowShader.UseShaderProgram();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D,depthMap);

		for(unsigned int i=0;i<NR_POINT_LIGHTS;i++)
		{
			glActiveTexture(GL_TEXTURE5 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP,pointLightDepthMap[i]);
		}

		float time = static_cast<float>(glfwGetTime());
		
		//  LightPositions[0].x = 1.0f*glm::sin(glm::radians(time*10.0f));
		//  LightPositions[0].z = 1.0f*glm::cos(glm::radians(time*10.0f));
		// LightingShader.setVec3("spotLight.position", MainCamera.GetCameraPos());
		// LightingShader.setVec3("spotLight.direction", MainCamera.GetCameraFront());

		LightingShadowShader.setVec3("viewPos",MainCamera.GetCameraPos());
		
		// for(unsigned int i = 0 ; i<10 ; i++)
		// {
		// 	glm::mat4 _model = glm::mat4(1.0f);
		// 	_model = glm::translate(_model,cubePositions[i]);
        // 	LightingShader.setTransformation("mat_Model",_model);
		// 	glDrawArrays(GL_TRIANGLES,0,36);
		// }
		// writing stencil on models

		LightingShadowShader.setInt("hasNormalMap",1);
		
		glActiveTexture(GL_TEXTURE13);
		glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap.id);
		glActiveTexture(GL_TEXTURE0);

		for(auto& model : SceneModels)
		{
			model->Draw(LightingShadowShader, GL_TRIANGLES);
		}

		LightingShadowShader.setInt("hasNormalMap",0);

		// Drawing debug normals gizmos
		
		// ExplosionShader.UseShaderProgram();
		// ourModel->Draw(ExplosionShader, GL_TRIANGLES);

		// Draw Asteriods
		RenderAsteriods(asteriodModel,&InstanceShader);



		// Drawing lightsource cubes and highlight

		
		glBindVertexArray(VAO);

		LightingShadowShader.UseShaderProgram();
		
	    // Enable writing to stencil buffer

		glStencilFunc(GL_ALWAYS,1,0xFF);
		glStencilMask(0xFF);

		// Rendering cubes

		// LightingShadowShader.setInt("material.texture_diffuse1", 0);
		// glActiveTexture(GL_TEXTURE0);
	    // glBindTexture(GL_TEXTURE_2D, woodTexture.id);

		// // LightingShadowShader.setInt("material.texture_specular1", 1);
		// // glActiveTexture(GL_TEXTURE1);
	    // // glBindTexture(GL_TEXTURE_2D, woodTexture.id);
		// glActiveTexture(GL_TEXTURE1);
	    // glBindTexture(GL_TEXTURE_2D, 0);
		// for(unsigned int i = 0 ; i<4 ; i++)
		// {
		// 	glm::mat4 _model = glm::mat4(1.0f);
		// 	_model = glm::translate(_model,pointLightPositions[i]);
        // 	LightingShadowShader.setTransformation("mat_Model",_model);
		// 	glDrawArrays(GL_TRIANGLES,0,36);
		// }

		// glm::mat4 _model = glm::mat4(1.0f);
		// _model = glm::translate(_model,glm::vec3(0.0f,-2.0f,0.0f));
		// _model = glm::scale(_model,glm::vec3(30.0f,0.1f,30.0f));
		// LightingShadowShader.setTransformation("mat_Model", _model);
		// glDrawArrays(GL_TRIANGLES, 0, 36);

		// // _model = glm::mat4(1.0f);
		// // _model = glm::translate(_model, glm::vec3(0.0f, 5.0f, 0.0f));
		// // _model = glm::scale(_model, glm::vec3(30.0f, 0.1f, 30.0f));
		// // LightingShadowShader.setTransformation("mat_Model", _model);
		// // glDrawArrays(GL_TRIANGLES, 0, 36);

		// glBindTexture(GL_TEXTURE_2D,0);

		// drawing Transparent objects
        // glStencilMask(0x00);
		// ImageShader.UseShaderProgram();
        
		// for(unsigned int i=0; i < 10 ; i++)
		// {
		// 	CubesGameObject[i]->GetComponent<Model>()->Draw(ImageShader,GL_TRIANGLES);
		// }
        
		// Disable writing to stencil buffer and just reading its values

		glStencilFunc(GL_NOTEQUAL,1,0xFF);
		glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_ALWAYS);

		//Drawing highlight cubes
		// glBindVertexArray(lightVAO);

		// HighlightShader.UseShaderProgram();

		// for(unsigned int i = 0 ; i<4 ; i++)
		// {
		// 	glm::mat4 _model = glm::mat4(1.0f);
		// 	_model = glm::translate(_model,pointLightPositions[i]);
		// 	_model = glm::scale(_model,glm::vec3(1.1f,1.1f,1.1f));
        // 	HighlightShader.setTransformation("mat_Model",_model);
		// 	glDrawArrays(GL_TRIANGLES,0,36);
		// }
		glEnable(GL_DEPTH_TEST);

		// RenderSkybox 
		
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

		// Multisampling 
		glBindFramebuffer(GL_READ_FRAMEBUFFER,msbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);

		glBlitFramebuffer(0,0,SRC_WIDTH,SRC_HEIGHT,0,0,SRC_WIDTH,SRC_HEIGHT,GL_COLOR_BUFFER_BIT,GL_NEAREST);
		glDisable(GL_DEPTH_TEST);

		// Brightness processing
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
		BrightShader.UseShaderProgram();
		BrightShader.setInt("screenTexture",0);
        
		unsigned int Color1[1] = {GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1,Color1);

		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,colorBuffer.id);
        glDrawArrays(GL_TRIANGLES,0,6);
		
		unsigned int Color0[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1,Color0);

        glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		// Blurring Brightness
		bool Horizontal = true, isFirstIteration = true;
		unsigned int samples = 10;
        BloomShader.UseShaderProgram();
		glBindVertexArray(quadVAO);

		for(unsigned int i=0; i<samples; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBO[Horizontal]);
			BloomShader.setInt("horizontal",Horizontal);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,isFirstIteration ? brightColorBuffer.id : pingpongBuffers[!Horizontal].id);
            glDrawArrays(GL_TRIANGLES,0,6);
			Horizontal = !Horizontal;

			isFirstIteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);


		// Post processing

		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.1f,0.1f,0.1f,0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// unsigned int attachMents[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		// glDrawBuffers(2,attachMents);

        PostShader.UseShaderProgram();
		PostShader.setFloat("exposure",0.4f);
		PostShader.setInt("scene",0);
		PostShader.setInt("bloomBlur",1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,colorBuffer.id);
		glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,pingpongBuffers[0].id);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(quadVAO);
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
	glBindBuffer(GL_UNIFORM_BUFFER,0);
}


