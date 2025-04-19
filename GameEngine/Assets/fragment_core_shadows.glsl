#version 460 core

#define NR_POINT_LIGHTS 4  
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
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
uniform sampler2D shadowMap;
uniform int hasNormalMap;
uniform samplerCube pointShadowMap[NR_POINT_LIGHTS];
float shadowCalculations(vec4 lightFragPos);
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

uniform PointLight pointLights[NR_POINT_LIGHTS];
float pointShadowCalculations(PointLight light,samplerCube depthMap);
vec3 pointShadowCalculationsDebugger(PointLight light,samplerCube depthMap);

vec3 CalcPointLight(PointLight light,int index);

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
bool m_hasNormalMap;

out vec4 FragColor;
in vec3 FragPos;
in vec3 normal;
in vec3 lightPos;
in vec4 FragPosLightSpace;
in mat3 TBN;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
float offsetSamples = 20.0;
float diskRadius = 0.05;

void main()
{
    vec3 result = vec3(0.0);
    result += CalcDirLight(dirLight);

    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
       //result += CalcPointLight(pointLights[i],i);
    //result += CalcSpotLight(spotLight);

    FragColor = vec4(result,1.0);
    //FragColor = vec4(pointShadowCalculationsDebugger(pointLights[0],pointShadowMap[0]),1.0);
}

vec3 CalcDirLight(DirLight light)
{  
    if(m_hasNormalMap)
    {
        vec3 normal = texture(material.texture_normal1,TextCords).rgb;
        normal = normal * 2.0 - 1.0; 
        normal = normalize(TBN*normal);
    }  
    vec3 normal = texture(material.texture_normal1,TextCords).rgb;
    normal = normal * 2.0 - 1.0; 
    normal = normalize(TBN*normal);
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(-light.direction);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 halfViewDirection = normalize(viewDirection + lightRay);

    //float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float spec = pow(max(dot(halfViewDirection,normal),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);
    float shadow = shadowCalculations(FragPosLightSpace);

    vec3 diffusion = light.diffuse * diff* texture(material.texture_diffuse1,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.texture_specular1,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.texture_diffuse1,TextCords).rgb;

    return ( (1.0 - shadow)*(diffusion +  specular) + ambientLight);
}

vec3 CalcPointLight(PointLight light,int index)
{
    vec3 normalSurface = normalize(normal);
    vec3 lightRay = normalize(light.position - FragPos);
    vec3 reflectRay = reflect(-lightRay,normal);
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 halfViewDirection = normalize(viewDirection + lightRay);

    //float spec = pow(max(dot(reflectRay,viewDirection),0.0),material.shininess);
    float spec = pow(max(dot(halfViewDirection,normal),0.0),material.shininess);
    float diff = max(dot(normalSurface,lightRay),0.0);

    vec3 diffusion = light.diffuse * diff* texture(material.texture_diffuse1,TextCords).rgb;
    vec3 specular = light.specular * spec* texture(material.texture_specular1,TextCords).rgb;
    vec3 ambientLight = light.ambient *  texture(material.texture_diffuse1,TextCords).rgb;
    
    float fragToLightDist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*fragToLightDist + light.quadratic * fragToLightDist*fragToLightDist);

    specular *= attenuation;
    diffusion *= attenuation;
    ambientLight *= attenuation;

    float shadow = pointShadowCalculations(light,pointShadowMap[index]);

    return ((1.0 - shadow)*(specular + diffusion) + ambientLight);
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
float shadowCalculations(vec4 fragPosLightSpace)
{
    vec3 proj = fragPosLightSpace.xyz/fragPosLightSpace.w;
    proj = proj*0.5 + 0.5;

    float closestDepth = texture(shadowMap,proj.xy).r;
    float currentDepth = proj.z;
    vec3 lightRay = normalize(-dirLight.direction);
    vec3 normalSurface = normalize(normal);
    float bias = max(0.05 * (1.0 - dot(lightRay,normalSurface)),0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0/textureSize(shadowMap,0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap,proj.xy + vec2(x,y)*texelSize).r;
            shadow += (currentDepth > pcfDepth + 0.0005) ? 1.0 : 0.0;
        }
    }

    //float shadow = (currentDepth > closestDepth + 0.0005) ? 1.0 : 0.0;
    if(proj.z > 1.0)
    shadow = 0.0;

    shadow /= 9.0;
    
    return shadow;
}

float pointShadowCalculations(PointLight light,samplerCube depthMap)
{
    vec3 FragToLight = FragPos - light.position;

    //float closestDepth = texture(depthMap,FragToLight).r;
    //closestDepth *= far_plane;
    float currentDepth = length(FragToLight);

    //float bias = max(0.05 * (1.0 - dot(lightRay,normalSurface)),0.005);

    float shadow = 0.0;
    float bias = 0.05;


    for(int i=0; i<offsetSamples; ++i)
    {
        float closestDepth = texture(depthMap,FragToLight + sampleOffsetDirections[i]*diskRadius).r;
        closestDepth *= far_plane;

        if(closestDepth <= far_plane) 
           shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
    }

    return shadow/offsetSamples;
}

vec3 pointShadowCalculationsDebugger(PointLight light,samplerCube depthMap)
{
    vec3 FragToLight = FragPos - light.position;

    float closestDepth = texture(depthMap,FragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(FragToLight);

    //float bias = max(0.05 * (1.0 - dot(lightRay,normalSurface)),0.005);

    float shadow = 0.0;
    float bias = 0.0005;

    return vec3(closestDepth/far_plane);
}