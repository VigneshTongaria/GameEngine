#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 mat_Model;
layout (std140) uniform Matrices
{
    mat4 mat_Projection;
    mat4 mat_View;
};

void main()
{
    gl_Position = mat_Projection*mat_View* mat_Model * vec4(aPos.x,aPos.y, aPos.z, 1.0);
}