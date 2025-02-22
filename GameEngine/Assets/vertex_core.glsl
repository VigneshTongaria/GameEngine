#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCords;

out vec3 FragPos;
out vec3 lightPos;
out vec3 normal;
out vec2 TextCords;
uniform mat4 mat_View;
uniform mat4 mat_Model;
uniform mat4 mat_Projection;
uniform vec3 lightPosition;

void main()
{
    gl_Position = mat_Projection*mat_View* mat_Model * vec4(aPos.x,aPos.y, aPos.z, 1.0);
    FragPos = vec3(mat_View*mat_Model * vec4(aPos,1.0));

    normal = mat3(transpose(inverse(mat_View*mat_Model))) * aNormal;
    lightPos = vec3(mat_View*vec4(lightPosition,1.0));
    TextCords = aTextCords;
}