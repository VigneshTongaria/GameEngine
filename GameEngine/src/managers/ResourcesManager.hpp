#pragma once
#include "../Mesh.h"
#include <unordered_map>


struct CubeMap
{
    unsigned int id;
    std::vector<std::string> facesPath;
};

class ResourcesManager
{
private:
    static const std::string Texture_Diffuse_Name;
    static const std::string Texture_Specular_Name;
    static const std::string Texture_Normal_Name;
    static std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;

public:
    static unsigned int VerticesCount;
    static Texture loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath);
    static Texture loadTexture(const char* path,TEXTURE_TYPE type);
    static Texture loadTexture(GLenum format,GLenum internalFormat,int width,int height);
    static Texture loadTextureFromMemory(const std::string &directory,const unsigned char* path,int imageSize,TEXTURE_TYPE type);
    static CubeMap loadCubeMap(std::vector<std::string> facesPath);
    static std::string getTextureName(TEXTURE_TYPE type);
};
