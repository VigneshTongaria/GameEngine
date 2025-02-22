#version 460 core

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissiveMap;
    float shininess;
}; 
in vec2 TextCords;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cosTheta;
    float cosThetaOuter;
};

uniform Light light; 
  
uniform Material material;

out vec4 FragColor;
in vec3 FragPos;
in vec3 normal;
in vec3 lightPos;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
void main()
{
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(light.position - FragPos);
    //lightRay = normalize(lightRay - light.direction);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);

    vec3 diffusion = light.diffuse * diff* texture(material.diffuseMap,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.specularMap,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.diffuseMap,TextCords).rgb;
    vec3 emissive = texture(material.emissiveMap,TextCords).rgb;
    emissive = vec3(0.0);

    if (texture(material.specularMap, TextCords).r != 0.0) emissive = vec3(0.0);
    
    float fragToLightDist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*fragToLightDist + light.quadratic * fragToLightDist*fragToLightDist);

    float fragCosTheta = dot(lightRay,normalize(-light.direction));
    float spotlight = 0.0;

    if(fragCosTheta > light.cosTheta) 
    {
        spotlight = 1.0f;
    }
    else if (fragCosTheta > light.cosThetaOuter)
    {
        spotlight = clamp((fragCosTheta- light.cosThetaOuter) / (light.cosTheta - light.cosThetaOuter), 0.0, 1.0);
    }

    specular *= attenuation*spotlight;
    diffusion *= attenuation*spotlight;

    vec3 result = (diffusion +  specular + ambientLight + emissive);

    FragColor = vec4(result,1.0);
}