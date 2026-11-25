#pragma once

#include"../core/Component.h"
#include <vector>
#include "../core/OpenGLHeaders.h"
class Mesh;
class Shader;

class MeshRenderer : public Component
{
private:
    /* data */
public:
    MeshRenderer(/* args */);
    std::vector<Mesh> meshes;
    void draw(Shader &Shader,GLenum mode);
    ~MeshRenderer();
};
