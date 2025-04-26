#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
in vec2 TextCords;

uniform sampler2D screenTexture;
uniform float exposure;
const float gamma = 2.2;

void main()
{
    vec3 hdr = texture(screenTexture, TextCords).rgb;
    vec3 ldr = vec3(1.0) - exp(-hdr*exposure);
    //ldr = pow(ldr, vec3(1.0/gamma));

    FragColor = vec4(hdr,1.0);

    float brightness = dot(FragColor.rgb,vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 0.3) 
      BrightColor = FragColor;
    else 
      BrightColor = vec4(0.0,0.0,0.0,1.0);

    // FragColor = texture(screenTexture, TextCords);
}