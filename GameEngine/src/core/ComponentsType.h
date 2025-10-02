#pragma once

enum COMPONENT_TYPE
{
    MESH,DIRLIGHT,COLLIDER
};


template <typename T>
COMPONENT_TYPE GetComponentTypeEnum();

class DirLight;
template <>
inline COMPONENT_TYPE GetComponentTypeEnum<DirLight>() { return COMPONENT_TYPE::DIRLIGHT; }

class Model;
template <>
inline COMPONENT_TYPE GetComponentTypeEnum<Model>() { return COMPONENT_TYPE::MESH; }