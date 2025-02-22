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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);
void Mat_Calculations();
std::string loadShaderSRC(const char* filename);
float Arrow_vertical_Input = 0.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
glm::mat4 mouseScroll = glm::mat4(1.0f);
glm::mat4 Scale = glm::mat4(1.0f);
glm::mat4 Model = glm::mat4(1.0f);
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
	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window,KeyBoard::KeyCallback);
	glfwSetMouseButtonCallback(window,Mouse::MouseButtonCallback);
	glfwSetCursorPosCallback(window,Mouse::CursorButtonCallback);
	glfwSetScrollCallback(window,Mouse::MouseScrollCallback);

	//LoadTextures - 1
	int width,height,nrChannels;
	unsigned char *data = stbi_load("Assets/Images/container2.png",&width,&height,&nrChannels,0);

	unsigned int texture_1;
	glGenTextures(1,&texture_1);
	glBindTexture(GL_TEXTURE_2D,texture_1);
    
	//GENERATING MIPMAPS and setting interpolations
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	if (data)
    {
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
       glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       std::cout << "Failed to load texture - 1" << std::endl;
    }
	stbi_image_free(data);
    
	//Texture 2
	unsigned char* data1 = stbi_load("Assets/Images/container2_specular.png",&width,&height,&nrChannels,0);

	unsigned int texture_2;
	glGenTextures(1,&texture_2);
	glBindTexture(GL_TEXTURE_2D,texture_2);
    
	//GENERATING MIPMAPS and setting interpolations
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	if (data1)
    {
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
       glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       std::cout << "Failed to load texture - 2" << std::endl;
    }
	stbi_image_free(data1);

	unsigned char* data2 = stbi_load("Assets/Images/container2_emissive.jpg",&width,&height,&nrChannels,0);

	unsigned int texture_3;
	glGenTextures(1,&texture_3);
	glBindTexture(GL_TEXTURE_2D,texture_3);
    
	//GENERATING MIPMAPS and setting interpolations
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	if (data2)
    {
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
       glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       std::cout << "Failed to load texture - 3" << std::endl;
    }
	stbi_image_free(data2);

	//Run_Shaders();
	//Vertex shader
	Shader LightingShader("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	Shader LightnigSourceShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_lightSource.glsl");

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

	glm::vec3 LightPositions[] = {
		glm::vec3(1.2f, 1.0f, 2.0f)
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
	Model = glm::rotate(Model,glm::radians(-45.0f), glm::vec3(1.0f,0.0f,0.0f));

	Projection = glm::perspective(glm::radians(60.0f),800.0f/600.0f,0.1f,100.0f);
	LightingShader.setTransformation("mat_Projection",Projection);

	LightingShader.setInt("material.diffuseMap",0);
	LightingShader.setInt("material.specularMap",1);
	LightingShader.setInt("material.emissiveMap",2);
	LightingShader.setFloat("material.shininess",64.0f);

	LightingShader.setVec3("light.ambient",glm::vec3(0.1f));
	LightingShader.setVec3("light.diffuse",glm::vec3(0.5f));
	LightingShader.setVec3("light.specular",glm::vec3(1.0f));

	LightnigSourceShader.UseShaderProgram();
	LightnigSourceShader.setTransformation("mat_Projection",Projection);

	//camera

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
		glClearColor(0.2f, 0.3f, 0.3f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Assigning texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture_1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture_2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,texture_3);


		//draw shapes
		glBindVertexArray(VAO);
		LightingShader.UseShaderProgram();

		float time = float(glfwGetTime());
		glm::mat4 Rot_45 = glm::mat4(1.0);
		//Rot_45 = glm::rotate(Rot_45, glm::radians(time*10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //Model = glm::rotate(Model, glm::radians(time*10.0f)*0.001f, glm::vec3(0.5f, 1.0f, 0.0f));
		View = MainCamera.GetViewMatrix();
		//View = glm::lookAt(cameraPos,cameraFront,cameraUp);
		
		LightPositions[0].x = 10.0f*glm::sin(glm::radians(time*10.0f));
		LightPositions[0].z = 10.0f*glm::cos(glm::radians(time*10.0f));

        LightingShader.setTransformation("mat_View",View);
		// LightingShader.setFloat("mix",Arrow_vertical_Input);
		LightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        LightingShader.setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
		LightingShader.setVec3("lightPosition",LightPositions[0]);
		LightingShader.setVec3("viewPos",MainCamera.GetCameraPos());
		
		for(unsigned int i = 0 ; i<10 ; i++)
		{
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model,cubePositions[i]);
        	LightingShader.setTransformation("mat_Model",_model);
			glDrawArrays(GL_TRIANGLES,0,36);
		}

		glBindVertexArray(lightVAO);

		LightnigSourceShader.UseShaderProgram();

		glm::mat4 _model = glm::mat4(1.0f);
		_model = glm::translate(_model, LightPositions[0]);
		_model = glm::scale(_model,glm::vec3(0.3f,0.3f,0.3f));
		LightnigSourceShader.setTransformation("mat_Model", _model);
		LightnigSourceShader.setTransformation("mat_View",View);

        glDrawArrays(GL_TRIANGLES, 0, 36);

		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

		/*LightnigShader_1.UseShaderProgram();
		LightnigShader_1.setFloat("x_Offset",0.0f);
		glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)(3*sizeof(unsigned int)));*/
		//glDrawArrays(GL_TRIANGLES,3,3);


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

