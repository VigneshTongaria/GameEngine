#include "Mesh.h"
#include "managers/ResourcesManager.hpp"

Mesh :: Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->textures = textures;
    this->indices = indices;
    setUpMesh();
}

void Mesh::setUpMesh()
{
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1,&VBO);
   glGenBuffers(1,&EBO);

   glBindVertexArray(VAO);
   
   if(vertices.size() != 0)
   {
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
   }

   if(indices.size() != 0)
   {
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
   }

   glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(Vertex),(void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1,3,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,normal));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2,2,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,textcords));
   glEnableVertexAttribArray(2);

   glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for(unsigned int i=0 ; i<textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        TEXTURE_TYPE type = textures[i].type;
        std::string number,t_name;

        switch (type)
        {
        case TEXTURE_TYPE::DIFFUSE:
          number = std::to_string(diffuseNr++);
          t_name = ResourcesManager::getTextureName(DIFFUSE);
          break;
        case TEXTURE_TYPE::SPECULAR:
          number = std::to_string(specularNr++);
          t_name = ResourcesManager::getTextureName(SPECULAR);

        default:
          break;
        }
          
        
        shader.setInt("material." + t_name + number,i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    if(indices.size() != 0)
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

    else 
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
    
    glBindVertexArray(0);
}
