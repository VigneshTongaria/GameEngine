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
	unsigned char *data = stbi_load("Assets/Images/image_1.jpg",&width,&height,&nrChannels,0);

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
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
       glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
       std::cout << "Failed to load texture - 1" << std::endl;
    }
	stbi_image_free(data);
    
	//Texture 2
	unsigned char* data1 = stbi_load("Assets/Images/image_2.png",&width,&height,&nrChannels,0);

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

	//Run_Shaders();
	//Vertex shader
	Shader Shader_program("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	Shader Shader_program_1("Assets/vertex_core.glsl", "Assets/fragment_core_1.glsl");
	
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

	float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //vertex attrbitue pointer assigning all types of data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1,3,GL_FLOAT,false,8 * sizeof(float),(void*)(3*(sizeof(float))));
	glVertexAttribPointer(2,2,GL_FLOAT,false,8 * sizeof(float),(void*)(6*sizeof(float)));

	//enabling all the attribute array
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	//Assigning textures ID'S
	Shader_program.UseShaderProgram();
	Shader_program.setInt("Texture1",0);
	Shader_program.setInt("Texture2",1);
	

	/*glm::mat4 Rot_0 = glm::mat4(1.0);
	Rot_0 = glm::rotate(Rot_0, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Shader_program_1.UseShaderProgram();

	Shader_program_1.setTransformation("mat_Rotation", Rot_0);*/
	Model = glm::rotate(Model,glm::radians(-45.0f), glm::vec3(1.0f,0.0f,0.0f));
	View = glm::translate(View,glm::vec3(0.0f,0.0f,-3.0f));
	Projection = glm::perspective(glm::radians(60.0f),800.0f/600.0f,0.1f,100.0f);
	Shader_program.setTransformation("mat_Model",Model);
	Shader_program.setTransformation("mat_View",View);
	Shader_program.setTransformation("mat_Projection",Projection);

	while (!glfwWindowShouldClose(window))
	{
		//process inputs
		process_inputs(window);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Assigning texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture_1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture_2);


		//draw shapes
		glBindVertexArray(VAO);
		Shader_program.UseShaderProgram();

		float time = glfwGetTime();
		glm::mat4 Rot_45 = glm::mat4(1.0);
		//Rot_45 = glm::rotate(Rot_45, glm::radians(time*10.0f), glm::vec3(0.0f, 0.0f, 1.0f));


        Shader_program.setTransformation("mat_View",View);
		Shader_program.setTransformation("mat_Rotation", Rot_45);
		Shader_program.setTransformation("mat_Scale",Scale);
	
		Shader_program.setFloat("x_Offset",0.0f);
		Shader_program.setFloat("mix",Arrow_vertical_Input);
		//glDrawArrays(GL_TRIANGLES,0,3);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

		/*Shader_program_1.UseShaderProgram();
		Shader_program_1.setFloat("x_Offset",0.0f);
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
		View = glm::translate(View,glm::vec3(0.0f,0.0f,0.01f));
	}
	if(KeyBoard::Key(GLFW_KEY_S))
	{
		View = glm::translate(View,glm::vec3(0.0f,0.0f,-0.01f));
	}
	if(KeyBoard::Key(GLFW_KEY_A))
	{
		View = glm::translate(View,glm::vec3(0.01f,0.0f,0.0f));
	}
	if(KeyBoard::Key(GLFW_KEY_D))
	{
		View = glm::translate(View,glm::vec3(-0.01f,0.0f,0.0f));
	}
	if(Mouse::MouseButton(GLFW_MOUSE_BUTTON_1))
	{
		View = glm::translate(View,glm::vec3(0.01f* Mouse::getMouseDX(),0.01f*Mouse::getMouseDY(),0.0f));
	}
	// Extract current scale from Scale matrix
	glm::vec3 currentScale = glm::vec3(
		glm::length(glm::vec3(Scale[0])), // X scaleth
		glm::length(glm::vec3(Scale[1])), // Y scale
		glm::length(glm::vec3(Scale[2]))  // Z scale
	);

	// Modify scale based on mouse wheel input
	float scaleFactor = 1.0f + (0.01f * Mouse::getMouseWheelY()); // Scaling factor
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

