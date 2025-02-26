#include "Mesh.h"

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
   glBindBuffer(GL_ARRAY_BUFFER,VBO);
   glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(Vertex),(void*)0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1,3,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,normal));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2,2,GL_FLOAT,false,sizeof(Vertex),(void*)offsetof(Vertex,textcords));

   glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for(int i=0 ; i<textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string t_name = textures[i].type;
        std::string number;
        
        if(t_name == "texture_diffuse")
          number = std::to_string(diffuseNr++);
        else if(t_name == "texture_specualr")
          number = std::to_string(specularNr++);
        
        shader.setInt("material." + t_name + number,i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}
