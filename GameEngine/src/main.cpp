#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_inputs(GLFWwindow* window);
void Mat_Calculations();

int main()
{
	Mat_Calculations();

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

	while (!glfwWindowShouldClose(window))
	{
		//process inputs
		process_inputs(window);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT);
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

