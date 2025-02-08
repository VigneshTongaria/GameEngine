#define SHADER_H

#include <glad/glad.h>  // Include OpenGL headers
#include <string>
#include <iostream>

class Shader
{
private:
    std::string loadShaderSRC(const char* filename);
public:
    unsigned int m_ID; 
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setTransformation(const std::string &name, glm::mat4 Trans) const;
    Shader(const char* VertexShaderPath, const char* FragmentShaderPath);
    void UseShaderProgram();
    ~Shader();
};