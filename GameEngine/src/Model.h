#ifndef MODEL_H
#define MODEL_H
#include<iostream>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public :
   Model(const char* path);
   ~Model();
   void Draw(Shader &shader);

private:
   std::vector<Mesh> meshes;
   std::vector<Texture> textures_Loaded;
   std::string directory;
   void loadModel(std::string path);
   void processNode(aiNode* node,const aiScene *scene );
   Mesh processMesh(aiMesh* mesh,const aiScene *scene);
   std::vector<Texture> loadMaterialsTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
   unsigned int loadTexture(const char* filename,const std::string &directory);
};
#endif