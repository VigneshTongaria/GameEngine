#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // Include OpenGL headers
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
private:
    std::string loadShaderSRC(const char* filename);
public:
    unsigned int m_ID; 
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;
    void setTransformation(const std::string &name, glm::mat4 Trans) const;
    Shader(const char* VertexShaderPath, const char* FragmentShaderPath,const char* GeometryShaderPath = NULL);
    void UseShaderProgram();
};
#endif