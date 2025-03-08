#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP
#include "../Mesh.h"

class SceneManager
{
private:
    static const std::string Texture_Diffuse_Name;
    static const std::string Texture_Specular_Name;

public:
    static Texture loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath);
    static std::string getTextureName(TEXTURE_TYPE type);
};

#endif
