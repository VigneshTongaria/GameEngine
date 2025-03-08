#pragma once
#include<iostream>
#include <vector>
#include <unordered_map>
#include "../Mesh.h"

enum DEFAULT_MODEL
{
    CUBE,SPHERE,CYLINDER
};

class ModelData
{
    public : 
    ModelData() = default; 
    ModelData(std::vector<Vertex> data,DEFAULT_MODEL type);
    std::vector<Vertex> data;
    DEFAULT_MODEL type;
};

class GeometryData
{
private:
   static std::unordered_map<DEFAULT_MODEL,ModelData> defaultModelDatas;

   template<typename T>
   static std::vector<T> GetVectorFromFloatArr(const float arr[],unsigned int stride, unsigned int offset,unsigned int size)
   {
       std::vector<T> vecT;
       for (unsigned int i = offset; i < size; i = i+stride)
       {
        if constexpr (std::is_same<T, glm::vec3>::value)
        {
            vecT.push_back(T(arr[i], arr[i + 1], arr[i + 2]));
        }
        else if constexpr (std::is_same<T, glm::vec2>::value)
        {
            vecT.push_back(T(arr[i], arr[i + 1]));
        }
       }
       
       return vecT;
   }

   static std::vector<Vertex> GetVertexVecFromData(std::vector<glm::vec3> positions,
    std::vector<glm::vec3> normals,std::vector<glm::vec2> textCords);

public:
    // Data for cube
    static const float cubeVertices[];

    static ModelData GetModelData(DEFAULT_MODEL model);
};

