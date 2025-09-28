#include "ShaderManager.hpp"
#include "../rendering/Shader.h"
#include "../rendering/Light/DirLight.h"

std::vector<Shader*> ShaderManager::shaderCache;
std::unordered_map<SHADER_TYPE,std::unique_ptr<Shader>> ShaderManager::shaderLibrary;

ShaderManager::ShaderManager()
{

}

void ShaderManager::init()
{
    // Shader LightingShader("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	// Shader LightnigSourceShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_lightSource.glsl");
	// Shader ImageShader("Assets/vertex_core.glsl", "Assets/fragment_core_1.glsl");
	// Shader HighlightShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_highlight.glsl");
	// Shader PostShader("Assets/vertex_unlit.glsl", "Assets/fragment_Blending.glsl");
	// Shader CubeMapShader("Assets/vertex_cubeMap.glsl", "Assets/fragment_cubeMap.glsl");
	// Shader ExplosionShader("Assets/GeometryShaders/Vertex_unlit.glsl", 
	// 	"Assets/fragment_core_highlight.glsl","Assets/GeometryShaders/Geometry_normals.glsl");
	// Shader InstanceShader("Assets/vertex_Instance.glsl", "Assets/GeometryShaders/fragment_unlit.glsl");
	// Shader DepthMapShader("Assets/vertex_depthMap.glsl", "Assets/fragment_depthMap.glsl");
	// Shader LightingShadowShader("Assets/vertex_core_shadows.glsl", "Assets/fragment_core_shadows.glsl");
	// Shader PointLightingShadowShader("Assets/vertex_pointLight.glsl","Assets/GeometryShaders/fragment_PointLightMap.glsl",
	// 	"Assets/GeometryShaders/Geometry_PointLight.glsl");
	// Shader BrightShader("Assets/vertex_unlit.glsl", "Assets/fragment_brightness.glsl");
	// Shader BloomShader("Assets/vertex_unlit.glsl", "Assets/fragment_bloom.glsl");
    
	shaderLibrary.emplace(SHADER_TYPE::LIT,std::make_unique<Shader>("Assets/vertex_core.glsl", "Assets/fragment_core.glsl"));
	shaderLibrary.emplace(SHADER_TYPE::LIT_SHADOWS,std::make_unique<Shader>("Assets/vertex_core_shadows.glsl", "Assets/fragment_core_shadows.glsl"));
	shaderLibrary.emplace(SHADER_TYPE::DEPTH,std::make_unique<Shader>("Assets/vertex_depthMap.glsl", "Assets/fragment_depthMap.glsl"));

	Shader* LightingShader = shaderLibrary.at(SHADER_TYPE::LIT).get();
	Shader* DepthMapShader = shaderLibrary.at(SHADER_TYPE::DEPTH).get();
	Shader* LightingShadowShader = shaderLibrary.at(SHADER_TYPE::LIT_SHADOWS).get();

	// for(auto it = shaderLibrary.begin(); it!= shaderLibrary.end(); ++it)
	// {
	// 	shaderCache.push_back(&it->second);
	// }

    // shaderCache.push_back(&LightingShader);
	// shaderCache.push_back(&LightnigSourceShader);
	// shaderCache.push_back(&ImageShader);
	// shaderCache.push_back(&HighlightShader);
	// shaderCache.push_back(&PostShader);
	// shaderCache.push_back(&CubeMapShader);
	// shaderCache.push_back(&ExplosionShader);
	// shaderCache.push_back(&InstanceShader);
	// shaderCache.push_back(&LightingShadowShader);

	// Adding uniform buffer index
	unsigned int uniformVertexCoreIndex = glGetUniformBlockIndex(LightingShader->m_ID,"Matrices");
	// unsigned int uniformVertexSkyboxIndex = glGetUniformBlockIndex(CubeMapShader.m_ID,"Matrices");
	// unsigned int uniformVertexLightingSourceIndex = glGetUniformBlockIndex(LightnigSourceShader.m_ID,"Matrices");
	// unsigned int uniformVertexSimplendex = glGetUniformBlockIndex(ExplosionShader.m_ID,"Matrices");
	// unsigned int uniformVertexInstanceIndex = glGetUniformBlockIndex(InstanceShader.m_ID,"Matrices");
	unsigned int uniformVertexCoreShadowsIndex = glGetUniformBlockIndex(LightingShadowShader->m_ID,"Matrices");

	glUniformBlockBinding(LightingShader->m_ID,uniformVertexCoreIndex,0);
	// glUniformBlockBinding(CubeMapShader.m_ID,uniformVertexSkyboxIndex,0);
	// glUniformBlockBinding(LightnigSourceShader.m_ID,uniformVertexLightingSourceIndex,0);
	// glUniformBlockBinding(ExplosionShader.m_ID,uniformVertexSimplendex,0);
	// glUniformBlockBinding(InstanceShader.m_ID,uniformVertexInstanceIndex,0);
	glUniformBlockBinding(LightingShadowShader->m_ID,uniformVertexCoreShadowsIndex,0);
}
Shader* ShaderManager::getShader(SHADER_TYPE type) 
{
    return shaderLibrary[type].get();
}

void ShaderManager::setShaderDirLightProperties(SHADER_TYPE type,DirLight* light,bool setColors,bool setView)
{
    Shader* s = getShader(type);
	s->UseShaderProgram();

	s->setVec3("dirLight.direction", light->LightDir);
	
	if(setColors)
	{
		s->setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		s->setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s->setVec3("dirLight.specular", glm::vec3(0.4f, 0.4f, 0.4f));
	}

	if(setView)
	{
		s->setTransformation("mat_Lightspace",light->lightSpaceMattrix);
	}
}

void ShaderManager::setShaderPlanes(SHADER_TYPE type,float n_plane,float f_plane)
{
	Shader* s = getShader(type);
	s->UseShaderProgram();
	s->setFloat("far_plane",f_plane);
}

ShaderManager::~ShaderManager()
{
	for (auto shader : shaderCache) {
            delete shader;
        }
	
	shaderCache.clear();

	for (auto& shader : shaderLibrary) {
            delete &shader.second;
        }
	shaderLibrary.clear();
}