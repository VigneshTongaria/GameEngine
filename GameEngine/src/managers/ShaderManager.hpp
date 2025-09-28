#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
class Shader;
class DirLight;

enum SHADER_TYPE
{
    LIT,LIT_SHADOWS,UNLIT,DEPTH,BLOOM
};


static class ShaderManager
{
private:
    static std::vector<Shader*> shaderCache;

public:
    ShaderManager(/* args */);
    
    static std::unordered_map<SHADER_TYPE,Shader> shaderLibrary;
    static void init();
    static void initShader(const char* vPath,const char* fPath);
    static Shader* getShader(SHADER_TYPE type);
    static void setShaderDirLightProperties(SHADER_TYPE type, DirLight* light,bool setColors,bool setView);
    static void setShaderPlanes(SHADER_TYPE type,float n_plane,float f_plane);

    ~ShaderManager();
};
