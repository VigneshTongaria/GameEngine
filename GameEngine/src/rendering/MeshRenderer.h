#pragma once

#include"../core/Component.h"
#include <vector>
class Mesh;

class MeshRenderer : public Component
{
private:
    /* data */
public:
    MeshRenderer(/* args */);
    std::vector<Mesh> meshes;
    ~MeshRenderer();
};
