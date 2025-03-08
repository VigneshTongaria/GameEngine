#version 460 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emissiveMap;
    float shininess;
}; 
uniform Material material;
in vec2 TextCords;

out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(material.texture_diffuse1,TextCords);

    if(textureColor.a < 0.1) 
       discard;
    FragColor = textureColor;
}