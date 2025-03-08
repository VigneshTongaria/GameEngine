#ifndef RESOURCESMANAGER_HPP
#define RESOURCESMANAGER_HPP
#include "../Mesh.h"

class ResourcesManager
{
private:
    static const std::string Texture_Diffuse_Name;
    static const std::string Texture_Specular_Name;

public:
    static Texture loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath);
    static Texture loadTexture(const char* path,TEXTURE_TYPE type);
    static std::string getTextureName(TEXTURE_TYPE type);
};

#endif
