#include "Model.h"
#include "stb/stb_image.h"

 Model::Model(const char *path)
{
     loadModel(path);
}

void Model::Draw(Shader &shader)
{
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
    for(unsigned int i = 0; i<node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }

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
    for(int i=0; i< mesh->mNumVertices; i++)
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
    for(int i=0; i<mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i];
        for(int j=0; j<face->mNumIndices; j++)
        {
            indices.push_back(face->mIndices[j]);
        }
    }

    //Processing materials data
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse_maps = loadMaterialsTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
        textures.insert(textures.end(),diffuse_maps.begin(),diffuse_maps.end());

        std::vector<Texture> specular_maps = loadMaterialsTextures(material,aiTextureType_SPECULAR,"texture_specular");
        textures.insert(textures.end(),specular_maps.begin(),specular_maps.end());
    }

    return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::loadMaterialsTextures(aiMaterial* mat,aiTextureType type,std::string typeName)
{
    std::vector<Texture> textures;

    for(int i=0 ;i<mat->GetTextureCount(type); i++)
    {
        aiString path;
        bool tex_already_loaded = false;
        mat->GetTexture(type,i,&path);

        for(int i=0; i< textures_Loaded.size(); i++)
        {
            if(std::strcmp(textures_Loaded[i].path.C_Str(),path.C_Str()))
            {
               tex_already_loaded = true;
               textures.push_back(textures_Loaded[i]);
               break;
            }
        }

        if(!tex_already_loaded)
        {
            Texture texture;
            texture.id = loadTexture(path.C_Str(),directory);
            texture.type = typeName;
            texture.path = path;

            textures.push_back(texture);
            textures_Loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::loadTexture(const char* filename,const std::string &directory)
{
	unsigned int textureID = -1;
	int width,height,nrChannels;

    std::string path = std::string(filename);
    path = directory + "/" + path;

	unsigned char *data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);

	if (data)
    {
		GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// GENERATING MIPMAPS and setting interpolations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << "\n";
        std::cout << "Reason: " << stbi_failure_reason() << std::endl;
    }
	stbi_image_free(data);
    
	return textureID;
}
