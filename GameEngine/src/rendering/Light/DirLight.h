#pragma once
#include"../../core/Component.h"
#include<glm/glm.hpp>
#include"../FrameBuffers/ShadowRenderTarget.h"
#include "ILight.h"

class DirLight : public Component, public ILight
{
private:
   
    /* data */
public:
    DirLight();

    glm::vec3 LightDir;
    float n_plane,f_plane;
    glm::mat4 lightView,lightProj,lightSpaceMattrix;
    ShadowRenderTarget shadowRenderTarget;
    void update() override;

    ~DirLight();
};
