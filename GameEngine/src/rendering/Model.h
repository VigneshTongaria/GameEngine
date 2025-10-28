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
#include "MeshRenderer.h"

struct MeshHierarchyData
{
   MeshRenderer meshRenderer;
   glm::mat4 globalTransform;

   MeshHierarchyData(MeshRenderer m,glm::mat4 gT) : meshRenderer(m),globalTransform(gT) { };

   std::vector<MeshHierarchyData*> children;

   void addChild(MeshHierarchyData* data)
   {
       children.push_back(data);
   }

   GameObject* addMeshRendererToScene(Scene* scene,GameObject* parent = nullptr)
   {
        scene->addNewGameObjectToScene<MeshRenderer>(parent,Transform(globalTransform));
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
   void processNode(const aiNode* Parent,const aiNode* node,const aiScene *scene,MeshHierarchyData* parentMesh);
   Mesh processMesh(aiMesh* mesh,const aiScene *scene,glm::mat4 globalTransform);
   GameObject* processMeshData(MeshHierarchyData* meshData, Scene* scene,GameObject* parent = nullptr);
   std::vector<Texture> loadMaterialsTextures(const aiScene* scene,aiMaterial *mat,aiTextureType type,TEXTURE_TYPE tex_type);
};