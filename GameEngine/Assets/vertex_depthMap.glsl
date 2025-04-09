#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 mat_Model;

void main()
{
    gl_Position = lightSpaceMatrix * mat_Model * vec4(aPos, 1.0);
}  