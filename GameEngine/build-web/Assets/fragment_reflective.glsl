#version 460 core
out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;

uniform samplerCube reflection;
uniform vec3 viewPos;

void main()
{   
    vec3 incedentRay = normalize(FragPos - viewPos);
    vec3 reflectRay = reflect(incedentRay,normalize (normal));
    FragColor = vec4(texture(reflection, reflectRay).rgb,1.0);
}