#version 460 core
out vec4 FragColor;
  
in vec2 TextCords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TextCords)), 1.0);
}