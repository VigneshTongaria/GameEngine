#version 460 core
out vec4 FragColor;
  
in vec2 TextCords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = texture(screenTexture, TextCords);
}