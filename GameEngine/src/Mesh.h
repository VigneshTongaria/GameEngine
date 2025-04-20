#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textcords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

enum TEXTURE_TYPE
{
    DIFFUSE,SPECULAR,NORMAL,EMMISIVE,EMPTY
};

struct Texture
{
    unsigned int id;
    TEXTURE_TYPE type;
    std::string path;
    aiString aiPath;
    const unsigned char* data;
};

enum DRAW_MODE
{
    TRIANGLES,LINES,POINT,
};

class Mesh
{
public :
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;   
    unsigned int VertexArrayObject;
    
    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
    void Draw(Shader &Shader,GLenum mode);
    void AssignTextures(Shader &Shader);
private:
    unsigned int VAO,VBO,EBO;
    void setUpMesh();
};
#endif
