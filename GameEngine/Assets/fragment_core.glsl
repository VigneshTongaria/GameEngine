#version 460 core
out vec4 FragColor;
in vec4 pos;
in vec2 TextCord;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float mix;
uniform vec3 lightColor;
uniform vec3 objectColor;
void main()
{
    FragColor = vec4(lightColor*objectColor,1.0);
}