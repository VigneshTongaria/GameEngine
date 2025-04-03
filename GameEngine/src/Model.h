#ifndef MODEL_H
#define MODEL_H
#include<iostream>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Component.h"
#include "GameObject.h"
#include "managers/ResourcesManager.hpp"
#include "data/GeometryData.hpp"

class Model : public Component
{
public :
   Model(const char* path);
   Model(DEFAULT_MODEL model,std::vector<Texture> textures);
   ~Model();
   void Draw(Shader &shader,GLenum mode);
   void DrawInstanced(Shader &shader,GLenum mode,unsigned int NOFinstance);
   //void AddRigidbody();

private:
   glm::mat4 mat_model;
   std::vector<Mesh> meshes;
   std::vector<Texture> textures_Loaded;
   std::string directory;
   void loadModel(std::string path);
   void loadModel(DEFAULT_MODEL model,std::vector<Texture> textures);
   void processNode(aiNode* node,const aiScene *scene);
   Mesh processMesh(aiMesh* mesh,const aiScene *scene);
   std::vector<Texture> loadMaterialsTextures(aiMaterial *mat,aiTextureType type,TEXTURE_TYPE tex_type);
};
#endif