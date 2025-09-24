#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
class Shader;

static class ShaderManager
{
private:
    static std::vector<Shader*> shaderCache;

public:
    ShaderManager(/* args */);
    
    static std::unordered_map<Shader*,std::string> shaderLibrary;
    static void init();
    static void initShader(const char* vPath,const char* fPath);

    ~ShaderManager();
};
