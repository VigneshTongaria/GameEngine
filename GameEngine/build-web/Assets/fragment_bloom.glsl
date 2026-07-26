#version 460 core
out vec4 FragColor;
in vec2 TextCords;
uniform sampler2D brightTexture;
float samples = 5.0;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 texelSize = 1.0 / textureSize(brightTexture,0);
    
    vec3 result = texture(brightTexture,TextCords).rgb * weight[0];

    if(horizontal)
    {
       for(int i = 1; i<5; ++i)
       {
          result += texture(brightTexture,TextCords + vec2(texelSize.x*i,0.0)).rgb*weight[i];
          result += texture(brightTexture,TextCords - vec2(texelSize.x*i,0.0)).rgb*weight[i];
       }
    }
    else
    {
       for(int i = 1; i<5; ++i)
       {
          result += texture(brightTexture,TextCords + vec2(0.0,texelSize.y*i)).rgb*weight[i];
          result += texture(brightTexture,TextCords - vec2(0.0,texelSize.y*i)).rgb*weight[i];
       }
    }

    FragColor = vec4(result,1.0);
}