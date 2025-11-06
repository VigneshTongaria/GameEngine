#include"DirLight.h"
#include "../../core/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

DirLight::DirLight() : shadowRenderTarget(1024,1024)
{
    LightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
    n_plane = 1.0f;
    f_plane = 300.0f;

	lightProj = glm::ortho(-100.0f,100.0f,-100.0f,100.0f,n_plane, f_plane);

    lightView = glm::lookAt(-100.0f* LightDir,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    lightSpaceMattrix =  lightProj *lightView;

    renderShadows = true;
}

void DirLight::update()
{
    if(LightDir != gameObject->transform.rotationXYZ)
    {
        LightDir = gameObject->transform.rotationXYZ;
        lightView = glm::lookAt(-10.0f* LightDir,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        lightSpaceMattrix =  lightProj *lightView;
    }
}

DirLight::~DirLight()
{
    //delete(&shadowRenderTarget);
}