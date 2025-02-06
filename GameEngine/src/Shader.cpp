#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
private:
    /* data */
    std::string loadShaderSRC(const char* filename);
public:
    unsigned int m_ID;
    void UseShaderProgram();
	void setFloat(const std::string &name, float value) const;
	void setTransformation(const std::string &name, glm::mat4 Trans) const;
    Shader(const char* VertexShaderPath,const char* FragmentShaderPath);
    ~Shader();
};

Shader::Shader(const char* VertexShaderPath,const char* FragmentShaderPath)
{
    int success;
	char Infolog[512];

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderSRC = loadShaderSRC(VertexShaderPath);
	const GLchar* vertShaderChar = vertexShaderSRC.c_str();
	glShaderSource(vertexShader, 1, &vertShaderChar, NULL);

	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, Infolog);
		std::cout << VertexShaderPath << " Error with vertex shader complilaton " <<  Infolog;
	}

	//Fragment shader
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragment_shader_SRC = loadShaderSRC(FragmentShaderPath);
	const GLchar* fragShaderChar = fragment_shader_SRC.c_str();
	glShaderSource(fragment_shader, 1, &fragShaderChar, NULL);

	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, Infolog);
		std::cout << FragmentShaderPath << " Error with fragment shader complilaton for"<< Infolog;
	}

	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragment_shader);
	glLinkProgram(m_ID);

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, Infolog);
		std::cout << "Error with shader program complilaton " << Infolog;
	}

    glDeleteShader(vertexShader);
    glDeleteShader(fragment_shader);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID,name.c_str()), value);
}
void Shader::setTransformation(const std::string &name, glm::mat4 Trans) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID,name.c_str()),1,false,glm::value_ptr(Trans));
}

std::string Shader::loadShaderSRC(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;
	std::string ret = "";

	file.open(filename);
	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Could not open " << filename << std::endl;

	}
	file.close();
	return ret;
}

Shader::~Shader()
{

}

void Shader::UseShaderProgram()
{
    glUseProgram(m_ID);
}

