#include "ResourcesManager.hpp"
#include "stb/stb_image.h"

const std::string ResourcesManager::Texture_Diffuse_Name = "texture_diffuse";
const std::string ResourcesManager::Texture_Specular_Name = "texture_specular";
unsigned int ResourcesManager::VerticesCount = 0;

Texture ResourcesManager::loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath)
{
	int width,height,nrChannels;

    std::string path = static_cast<std::string>(filename);
    path = directory + "/" + path;

    Texture texture = loadTexture(path.c_str(),type);
    texture.aiPath = aiPath;
    
	return texture;
}

Texture ResourcesManager::loadTexture(const char* path,TEXTURE_TYPE type)
{
	int width,height,nrChannels;

    std::string pathName = static_cast<std::string>(path);

    Texture texture;
    texture.id = -1;
    texture.type = type;
    texture.path = pathName;

	unsigned char *data = stbi_load(path,&width,&height,&nrChannels,0);

	if (data)
    {
		GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        GLenum internalFormat = format;
        
        if(type == TEXTURE_TYPE::DIFFUSE)
        {
            if(nrChannels == 3) internalFormat = GL_SRGB;
            else if(nrChannels == 4) internalFormat = GL_SRGB_ALPHA;
        }

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// GENERATING MIPMAPS and setting interpolations
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << "\n";
        std::cout << "Reason: " << stbi_failure_reason() << std::endl;
    }
	stbi_image_free(data);
    
	return texture;
}

Texture ResourcesManager::loadTexture(GLenum format,int width,int height)
{
    Texture texture;
    texture.id = -1;
    texture.type = TEXTURE_TYPE::EMPTY;
    texture.path = "";
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // GENERATING MIPMAPS and setting interpolations

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

CubeMap ResourcesManager::loadCubeMap(std::vector<std::string> facesPath)
{
    CubeMap cubeMap;
    cubeMap.id = -1;
    cubeMap.facesPath = facesPath;

    glGenTextures(1,&cubeMap.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap.id);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < facesPath.size(); i++)
    {
        unsigned char *data = stbi_load(facesPath[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture: " << facesPath[i] << "\n";
            std::cout << "Reason: " << stbi_failure_reason() << std::endl;
        }
        stbi_image_free(data);
    }
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return cubeMap;
}

std::string ResourcesManager::getTextureName(TEXTURE_TYPE type)
{
    switch (type)
    {
    case TEXTURE_TYPE::DIFFUSE:
        return Texture_Diffuse_Name;
    case TEXTURE_TYPE::SPECULAR:
        return Texture_Specular_Name;
    
    default:
        std::cout<<"Texture type does not exist in current context"; 
        return "";
    }
}
