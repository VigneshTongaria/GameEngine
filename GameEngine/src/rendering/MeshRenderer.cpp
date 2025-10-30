#include "MeshRenderer.h"
#include "Mesh.h"
#include "../core/GameObject.h"

MeshRenderer::MeshRenderer()
{

}

void MeshRenderer::draw(Shader &shader,GLenum mode)
{
    for(auto& mesh: meshes)
    {
        mesh.Draw(shader,gameObject->transform.getTransformationMatrix(), mode);
    }
}

MeshRenderer::~MeshRenderer()
{

}