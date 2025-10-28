#pragma once

enum COMPONENT_TYPE
{
    MESH_RENDERER,DIRLIGHT,COLLIDER
};


template <typename T>
COMPONENT_TYPE GetComponentTypeEnum();

class DirLight;
template <>
inline COMPONENT_TYPE GetComponentTypeEnum<DirLight>() { return COMPONENT_TYPE::DIRLIGHT; }

class MeshRenderer;
template <>
inline COMPONENT_TYPE GetComponentTypeEnum<MeshRenderer>() { return COMPONENT_TYPE::MESH_RENDERER; }