#version 460 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 normal;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
    float ambientLight = 0.1f;
    float specularStrength = 0.5;
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(lightPos - FragPos);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);
    float specular = specularStrength* pow(max(dot(reflectRay,viewDirection),0.0),32);

    float diffusion = max(dot(normalSurface,lightRay),0.0);
    FragColor = vec4(((diffusion + ambientLight + specular)*lightColor)*objectColor,1.0);
}