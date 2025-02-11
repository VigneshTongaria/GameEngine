#version 460 core
out vec4 FragColor;
in vec4 pos;
in vec2 TextCord;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float mix;
void main()
{
    FragColor = mix(texture(Texture1, -TextCord), texture(Texture2, TextCord), mix);
}