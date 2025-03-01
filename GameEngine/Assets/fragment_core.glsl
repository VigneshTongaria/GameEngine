#version 460 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emissiveMap;
    float shininess;
}; 
in vec2 TextCords;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light); 

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light); 

struct SpotLight {
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

uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light); 
  
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
    vec3 result = CalcDirLight(dirLight);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i]);
    result += CalcSpotLight(spotLight);

    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light)
{
    
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(-light.direction);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);

    vec3 diffusion = light.diffuse * diff* texture(material.texture_diffuse1,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.texture_specular1,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.texture_diffuse1,TextCords).rgb;

    return (diffusion + specular + ambientLight);
}

vec3 CalcPointLight(PointLight light)
{
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(light.position - FragPos);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);

    vec3 diffusion = light.diffuse * diff* texture(material.texture_diffuse1,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.texture_specular1,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.texture_diffuse1,TextCords).rgb;
    
    float fragToLightDist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*fragToLightDist + light.quadratic * fragToLightDist*fragToLightDist);

    specular *= attenuation;
    diffusion *= attenuation;
    ambientLight *= attenuation;

    return (specular + diffusion + ambientLight);
}

vec3 CalcSpotLight(SpotLight light)
{
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(light.position - FragPos);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);

    vec3 diffusion = light.diffuse * diff* texture(material.texture_diffuse1,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.texture_specular1,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.texture_diffuse1,TextCords).rgb;
    
    float fragToLightDist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*fragToLightDist + light.quadratic * fragToLightDist*fragToLightDist);

    float fragCosTheta = dot(lightRay,normalize(-1.0 * light.direction));
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

    return (specular + diffusion + ambientLight);
}