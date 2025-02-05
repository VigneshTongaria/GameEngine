#version 460 core
out vec4 FragColor;
in vec4 color;

void main()
{
    FragColor = vec4(vec2(color.x,color.y),1.0f, 1.0f);
}