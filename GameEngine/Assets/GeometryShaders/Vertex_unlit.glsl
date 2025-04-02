#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCords;

out VS_OUT {
    vec2 TextCords;
    vec3 normal;
} vs_out;

layout (std140) uniform Matrices
{
    mat4 mat_Projection;
    mat4 mat_View;
};
uniform mat4 mat_Model;

void main()
{
    gl_Position = mat_Projection*mat_View* mat_Model * vec4(aPos.x,aPos.y, aPos.z, 1.0);

    vs_out.normal = mat3(transpose(inverse(mat_Model))) * aNormal;
    vs_out.TextCords = aTextCords;
}