#ifndef RESOURCESMANAGER_HPP
#define RESOURCESMANAGER_HPP
#include "../Mesh.h"

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

public:
    static unsigned int VerticesCount;
    static Texture loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath);
    static Texture loadTexture(const char* path,TEXTURE_TYPE type);
    static Texture loadTexture(GLenum format,int width,int height);
    static CubeMap loadCubeMap(std::vector<std::string> facesPath);
    static std::string getTextureName(TEXTURE_TYPE type);
};

#endif
