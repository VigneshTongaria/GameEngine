#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 mat_View;
uniform mat4 mat_Projection;

void main()
{
    TexCoords = aPos;
    gl_Position = mat_Projection * mat_View* vec4(aPos, 1.0);
}  