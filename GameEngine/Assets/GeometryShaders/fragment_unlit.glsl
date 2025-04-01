#version 460 core
out vec4 FragColor;

in vec2 textCords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emissiveMap;
    float shininess;
}; 
uniform Material material;

void main()
{
    FragColor = vec4(texture(material.texture_diffuse1, textCords).rgb,1.0);
}