#pragma once
#include<iostream>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../core/Component.h"
#include "../core/GameObject.h"
#include "../managers/ResourcesManager.hpp"
#include "../data/GeometryData.hpp"

struct MeshHierarchyData
{
   Mesh mesh;
   glm::mat4 globalTransform;

   MeshHierarchyData(Mesh m,glm::mat4 gT) : mesh(m),globalTransform(gT) { };

   std::vector<MeshHierarchyData*> children;

   void addChild(MeshHierarchyData* data)
   {
       children.push_back(data);
   }
};

class Model
{
public :
   Model(const char* path);
   Model(const char* path, unsigned int instances);
   Model(DEFAULT_MODEL model,Material mat,std::vector<Texture> textures);
   GameObject* addModelToScene(Scene* scene,GameObject* parent = nullptr);
   ~Model();
   void Draw(Shader &shader,GLenum mode);
   void DrawInstanced(Shader &shader,GLenum mode,unsigned int NOFinstance);
   //void AddRigidbody();

private:
   glm::mat4* instancesModels;
   std::vector<Mesh> meshes;
   std::vector<MeshHierarchyData> meshHierarchyDatas;
   std::string directory;
   aiScene modelData;
   void loadModel(std::string path);
   void loadModel(DEFAULT_MODEL model,Material mat,std::vector<Texture> textures);
   void processNode(aiNode* Parent, aiNode* node,const aiScene *scene);
   MeshHierarchyData processMesh(aiMesh* mesh,const aiScene *scene,glm::mat4 globalTransform);
   std::vector<Texture> loadMaterialsTextures(const aiScene* scene,aiMaterial *mat,aiTextureType type,TEXTURE_TYPE tex_type);
};