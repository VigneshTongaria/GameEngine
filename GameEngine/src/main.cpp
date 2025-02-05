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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);
void Mat_Calculations();
std::string loadShaderSRC(const char* filename);

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

	//Run_Shaders();
	//Vertex shader
	int success;
	char Infolog[512];
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSRC = loadShaderSRC("Assets/vertex_core.glsl");
	const GLchar* vertShaderChar = vertexShaderSRC.c_str();
	glShaderSource(vertexShader, 1, &vertShaderChar, NULL);

	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, Infolog);
		std::cout << "Error with vertex shader complilaton " << Infolog;
	}

	//Fragment shader
	unsigned int fragment_shader,fragment_shader_1;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragment_shader_SRC = loadShaderSRC("Assets/fragment_core.glsl");
	const GLchar* fragShaderChar = fragment_shader_SRC.c_str();
	glShaderSource(fragment_shader, 1, &fragShaderChar, NULL);

	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, Infolog);
		std::cout << "Error with fragment shader[0] complilaton " << Infolog;
	}

	unsigned int shader_program = glCreateProgram();

	glAttachShader(shader_program, vertexShader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	//running shader program

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, Infolog);
		std::cout << "Error with shader program complilaton " << Infolog;
	}

	fragment_shader_1 = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragment_shader_SRC_1 = loadShaderSRC("Assets/fragment_core_1.glsl");
	const GLchar* fragShaderChar_1 = fragment_shader_SRC_1.c_str();

	glShaderSource(fragment_shader_1, 1, &fragShaderChar_1, NULL);

	glCompileShader(fragment_shader_1);

	glGetShaderiv(fragment_shader_1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader_1, 512, NULL, Infolog);
		std::cout << "Error with fragment shader[1] complilaton " << Infolog;
	}

	unsigned int shader_program_1 = glCreateProgram();
    glAttachShader(shader_program_1, vertexShader);
	glAttachShader(shader_program_1, fragment_shader_1);
	glLinkProgram(shader_program_1);

	glDeleteShader(vertexShader);
	glDeleteShader(fragment_shader);
	glDeleteShader(fragment_shader_1);

	float vertices[] = {
		// positions    //colors
	 -0.3f,-0.1f,0.0f, 0.4f,0.6f,0.4f,
	 -0.1f,0.7f,0.0f,  0.3f,0.8f,0.9f,
	 0.8f,-0.8f,0.0f,  0.3f,0.8f,0.9f,
	 0.4f,0.3f,0.0f,    0.4f,0.6f,0.4f,
	};
	unsigned int indices[] = 
	{
		0,1,2, //first traingle
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

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1,3,GL_FLOAT,false,6 * sizeof(float),(void*)(3*(sizeof(float))));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		//process inputs
		process_inputs(window);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw shapes
		glBindVertexArray(VAO);
		glUseProgram(shader_program);
		//glDrawArrays(GL_TRIANGLES,0,3);
		glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);

		glUseProgram(shader_program_1);
		glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)(3*sizeof(unsigned int)));
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
	if (glfwGetKey(window, GLFW_KEY_1) == 1)
	{
		glfwSetWindowShouldClose(window, 1);
	}
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

