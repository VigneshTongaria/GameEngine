#include "Scene.h"

void Scene::init()
{
    const float* vertices = GeometryData::cubeVertices;
	const float* skyboxVertices = GeometryData::skyboxVertices;
	const float* quadVertices = GeometryData::quadVertices;

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
	// unsigned int VBO, VBO_1,VAO,VAO_1,EBO;
	// glGenVertexArrays(1, &VAO);
	// glGenVertexArrays(1, &VAO_1);
	// glGenBuffers(1, &VBO);
	// glGenBuffers(1, &VBO_1);
	// glGenBuffers(1,&EBO);
    
	// //Binding buffers
	// glBindVertexArray(VAO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    // //vertex attrbitue pointer assigning all types of data
	// glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	// glVertexAttribPointer(1,3,GL_FLOAT,false,8 * sizeof(float),(void*)(3*(sizeof(float))));
	// glVertexAttribPointer(2,2,GL_FLOAT,false,8 * sizeof(float),(void*)(6*(sizeof(float))));
	// //enabling all the attribute array
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);
	// glEnableVertexAttribArray(2);

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

	// unsigned int lightVAO;
	// glGenVertexArrays(1, &lightVAO);
	// glBindVertexArray(lightVAO);
	// // we only need to bind to the VBO, the container's VBO's data already contains the data.
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// // set the vertex attribute
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);

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

	unsigned int attachMents[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2,attachMents);

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

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

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

	// Generating uniform buffers
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBufferRange(GL_UNIFORM_BUFFER,0, uboMatrices, 0, 3 * sizeof(glm::mat4));

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}