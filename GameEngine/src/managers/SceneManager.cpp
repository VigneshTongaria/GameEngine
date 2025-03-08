#include "SceneManager.hpp"
#include "stb/stb_image.h"

const std::string SceneManager::Texture_Diffuse_Name = "texture_diffuse";
const std::string SceneManager::Texture_Specular_Name = "texture_specular";

Texture SceneManager::loadTexture(const char* filename,const std::string &directory,TEXTURE_TYPE type,aiString aiPath)
{
	int width,height,nrChannels;

    std::string path = static_cast<std::string>(filename);
    path = directory + "/" + path;

    Texture texture;
    texture.id = -1;
    texture.type = type;
    texture.path = path;
    texture.aiPath = aiPath;

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

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);

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
    
	return texture;
}

std::string SceneManager::getTextureName(TEXTURE_TYPE type)
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
