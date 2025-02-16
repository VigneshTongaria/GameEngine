#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec2 TextCord;
out vec3 normal;
uniform mat4 mat_View;
uniform mat4 mat_Model;
uniform mat4 mat_Projection;

void main()
{
    gl_Position = mat_Projection*mat_View* mat_Model * vec4(aPos.x,aPos.y, aPos.z, 1.0);
    FragPos = vec3(mat_Model * vec4(aPos,1.0));

    normal = mat3(transpose(inverse(mat_Model))) * aNormal;
}