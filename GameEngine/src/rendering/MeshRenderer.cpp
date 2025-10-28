#include "MeshRenderer.h"
#include "Mesh.h"

void MeshRenderer::draw(Shader &shader,GLenum mode)
{
    for(auto& mesh: meshes)
    {
        mesh.Draw(shader,mode);
    }
}