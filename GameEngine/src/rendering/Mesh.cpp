#include "Mesh.h"
#include "../managers/ResourcesManager.hpp"
#include "../core/GameObject.h"

Mesh::Mesh(std::vector<Vertex> vertices,Material material, std::vector<Texture> textures, std::vector<unsigned int> indices)
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

void Mesh::Draw(Shader &shader,const glm::mat4& transformation, GLenum mode)
{
    // GameObject *go = this->gameObject;
    // if (go == nullptr)
    //     std::cout << "Gameobject is null" << std::endl;
    
    mat_model = transformation;
    // mat_model = glm::translate(mat_model, _t->position);

    // if (_t->rotationXYZ.x != 0)
    // {
    //     mat_model = glm::rotate(mat_model, glm::radians(_t->rotationXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // }
    // if (_t->rotationXYZ.y != 0)
    // {
    //     mat_model = glm::rotate(mat_model, glm::radians(_t->rotationXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f));
    // }
    // if (_t->rotationXYZ.z != 0)
    // {
    //     mat_model = glm::rotate(mat_model, glm::radians(_t->rotationXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // }

    // mat_model = glm::scale(mat_model, _t->scale);

    shader.setTransformation("mat_Model",mat_model);
    shader.setTransformation("transpose_mat_Model",glm::transpose(glm::inverse(mat_model)));
    
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

        glBindTexture(GL_TEXTURE_2D, 0);

        TEXTURE_TYPE type = textures[i].type;
        std::string number,t_name;

        shader.setBool("hasNormalMap",false);

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
          shader.setBool("hasNormalMap",true);
          break;

        default:
          break;
        }
        shader.setInt("material." + t_name + number,i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

}
