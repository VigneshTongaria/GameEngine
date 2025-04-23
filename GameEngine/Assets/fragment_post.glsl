#version 460 core
out vec4 FragColor;
  
in vec2 TextCords;

uniform sampler2D screenTexture;
uniform float exposure;
const float gamma = 2.2;

void main()
{
    vec3 hdr = texture(screenTexture, TextCords).rgb;
    vec3 ldr = vec3(1.0) - exp(-hdr*exposure);
    //ldr = pow(ldr, vec3(1.0/gamma));

    FragColor = vec4(ldr,1.0);

    // FragColor = texture(screenTexture, TextCords);
}