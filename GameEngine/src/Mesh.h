#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#include <vector>
#include "Shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textcords;
};

struct Texture
{
    unsigned int id;
    std::string type;
};


class Mesh
{
public :
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;   
    
    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
    void Draw(Shader &Shader);

private:
    unsigned int VAO,VBO,EBO;
    void setUpMesh();
};
#endif
