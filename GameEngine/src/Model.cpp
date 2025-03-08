#include "Model.h"
#include "stb/stb_image.h"

Model::Model(const char* path)
{
     loadModel(path);
}
Model::Model(DEFAULT_MODEL model,std::vector<Texture> textures)
{
     loadModel(model,textures);
}
Model::~Model()
{
    for(int i=0; i<textures_Loaded.size(); i++)
    {
        glDeleteTextures(1,&textures_Loaded[i].id);
    }
}

void Model::Draw(Shader &shader)
{
    GameObject *go = this->gameObject;
    if (go == nullptr)
        std::cout << "Gameobject is null" << std::endl;
    mat_model = glm::mat4(1.0f);
    mat_model = glm::translate(mat_model, go->position);

    if (go->rotationXYZ.x != 0)
    {
        mat_model = glm::rotate(mat_model, glm::radians(go->rotationXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (go->rotationXYZ.y != 0)
    {
        mat_model = glm::rotate(mat_model, glm::radians(go->rotationXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (go->rotationXYZ.z != 0)
    {
        mat_model = glm::rotate(mat_model, glm::radians(go->rotationXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    mat_model = glm::scale(mat_model, go->scale);

    shader.setTransformation("mat_Model",mat_model);
    
    for(unsigned int i=0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_FlipUVs|aiProcess_Triangulate);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() <<std::endl;
        return;
    }
    std::cout << "SUCCESS::ASSIMP::Scene loaded succesfully"<<std::endl;
    directory = path.substr(0,path.find_last_of('/'));

    processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode* node,const aiScene* scene)
{
    //std::cout<<"Number of meshes in node - "<<node->mNumMeshes<<std::endl;
    for(unsigned int i = 0; i<node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }
   // std::cout<<"Number of children in node - "<<node->mNumChildren<<std::endl;
    for(unsigned int i=0; i<node->mNumChildren; i++)
    {
        processNode(node->mChildren[i],scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh,const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;  
    
    //Processing vertices data
    for(unsigned int i=0; i< mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
        
        if(mesh->HasTextureCoords(0))
        {
            vertex.textcords = glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);
        }
        else vertex.textcords = glm::vec2(0.0f,0.0f);
        vertices.push_back(vertex);
    }
    
    //Processing indices data
    for(unsigned int i=0; i<mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i];
        for(unsigned int j=0; j<face->mNumIndices; j++)
        {
            indices.push_back(face->mIndices[j]);
        }
    }

    //Processing materials data
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse_maps = loadMaterialsTextures(material,aiTextureType_DIFFUSE,TEXTURE_TYPE::DIFFUSE);
        textures.insert(textures.end(),diffuse_maps.begin(),diffuse_maps.end());

        std::vector<Texture> specular_maps = loadMaterialsTextures(material,aiTextureType_SPECULAR,TEXTURE_TYPE::SPECULAR);
        textures.insert(textures.end(),specular_maps.begin(),specular_maps.end());
    }

    return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::loadMaterialsTextures(aiMaterial* mat,aiTextureType type,TEXTURE_TYPE t_type)
{
    std::vector<Texture> textures;

    for(unsigned int i=0 ;i<mat->GetTextureCount(type); i++)
    {
        aiString path;
        bool tex_already_loaded = false;
        mat->GetTexture(type,i,&path);
       // std::cout << "Texture - " << typeName << "path - "<< path.C_Str() << std::endl;

        for(int j=0; j< textures_Loaded.size(); j++)
        {
            if(textures_Loaded[j].aiPath == path)
            {
              // std::cout<<textures_Loaded[j].path.C_Str()<<" Texture already exists so assigning old texture to this mesh"<<std::endl;
               tex_already_loaded = true;
               textures.push_back(textures_Loaded[j]);
               break;
            }
        }

        if(tex_already_loaded == false)
        {
            Texture texture= ResourcesManager::loadTexture(path.C_Str(),directory,t_type,path);

            textures.push_back(texture);
            textures_Loaded.push_back(texture);
           // std::cout << "Texture - " << typeName << "Loaded"<<std::endl;
        }
    }

    return textures;
}

void Model::loadModel(DEFAULT_MODEL model,std::vector<Texture> textures)
{
     std::vector<Vertex> vertices;
     std::vector<unsigned int> indices;

     switch (model)
     {
     case DEFAULT_MODEL::CUBE:
        vertices = GeometryData::GetModelData(DEFAULT_MODEL :: CUBE).data;
        break;
     
     default:
        break;
     }
     meshes.push_back(Mesh(vertices,textures,indices));
}
