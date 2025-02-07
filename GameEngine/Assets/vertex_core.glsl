#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCord;
out vec4 color;
out vec4 pos;
out vec2 TextCord;
uniform float x_Offset;
uniform mat4 mat_Rotation;

void main()
{
    gl_Position = mat_Rotation * vec4(aPos.x + x_Offset,aPos.y, aPos.z, 1.0);
    pos = gl_Position;

    color = vec4(aColor,1.0);
    TextCord = aTextCord;
}