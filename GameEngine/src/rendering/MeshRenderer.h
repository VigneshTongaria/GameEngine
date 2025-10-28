#pragma once

#include"../core/Component.h"
#include <vector>
#include <glad/glad.h>
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
