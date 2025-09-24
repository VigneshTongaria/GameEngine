#include "ShaderManager.hpp"
#include "../rendering/Shader.h"

std::vector<Shader*> ShaderManager::shaderCache;
std::unordered_map<Shader*,std::string> ShaderManager::shaderLibrary;

void ShaderManager::init()
{
    Shader LightingShader("Assets/vertex_core.glsl", "Assets/fragment_core.glsl");
	Shader LightnigSourceShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_lightSource.glsl");
	Shader ImageShader("Assets/vertex_core.glsl", "Assets/fragment_core_1.glsl");
	Shader HighlightShader("Assets/vertex_core_lightSource.glsl", "Assets/fragment_core_highlight.glsl");
	Shader PostShader("Assets/vertex_unlit.glsl", "Assets/fragment_Blending.glsl");
	Shader CubeMapShader("Assets/vertex_cubeMap.glsl", "Assets/fragment_cubeMap.glsl");
	Shader ExplosionShader("Assets/GeometryShaders/Vertex_unlit.glsl", 
		"Assets/fragment_core_highlight.glsl","Assets/GeometryShaders/Geometry_normals.glsl");
	Shader InstanceShader("Assets/vertex_Instance.glsl", "Assets/GeometryShaders/fragment_unlit.glsl");
	Shader DepthMapShader("Assets/vertex_depthMap.glsl", "Assets/fragment_depthMap.glsl");
	Shader LightingShadowShader("Assets/vertex_core_shadows.glsl", "Assets/fragment_core_shadows.glsl");
	Shader PointLightingShadowShader("Assets/vertex_pointLight.glsl","Assets/GeometryShaders/fragment_PointLightMap.glsl",
		"Assets/GeometryShaders/Geometry_PointLight.glsl");
	Shader BrightShader("Assets/vertex_unlit.glsl", "Assets/fragment_brightness.glsl");
	Shader BloomShader("Assets/vertex_unlit.glsl", "Assets/fragment_bloom.glsl");

    shaderCache.push_back(&LightingShader);
	shaderCache.push_back(&LightnigSourceShader);
	shaderCache.push_back(&ImageShader);
	shaderCache.push_back(&HighlightShader);
	shaderCache.push_back(&PostShader);
	shaderCache.push_back(&CubeMapShader);
	shaderCache.push_back(&ExplosionShader);
	shaderCache.push_back(&InstanceShader);
	shaderCache.push_back(&LightingShadowShader);
}
void ShaderManager::initShader(const char* vPath,const char* fPath)
{
      
}