#include "Mesh.h"
#include "managers/ResourcesManager.hpp"

Mesh :: Mesh(std::vector<Vertex> vertices,Material material, std::vector<Texture> textures, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->material = material;
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
   glVertexAttribPointer(3,3,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,tangent));
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(4,3,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,bitangent));
   glEnableVertexAttribArray(4);

   glBindVertexArray(0);

   VertexArrayObject = VAO;
}

void Mesh::Draw(Shader &shader,GLenum mode)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
      
    AssignTextures(shader);
    glBindVertexArray(VAO);

    if (indices.size() != 0)
      glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);

    else
      glDrawArrays(mode, 0, vertices.size());

    glBindVertexArray(0);

    for(unsigned int i=0 ; i<textures.size(); i++)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D,0);
    }

    // Calculate vertices count
    ResourcesManager::VerticesCount +=  vertices.size();
}

void Mesh::AssignTextures(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    shader.setFloat("material.shininess",material.shininess);
      
    for(unsigned int i=0 ; i<textures.size(); i++)
    {
        if(textures[i].id == -1)
        {
           std::cout<<"Texture not assigned" << "\n";
           continue;
        }

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
          break;

        case TEXTURE_TYPE::NORMAL:
          number = std::to_string(normalNr++);
          t_name = ResourcesManager::getTextureName(NORMAL);
          break;

        default:
          break;
        }
        shader.setInt("material." + t_name + number,i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

}
