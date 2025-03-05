#version 460 core
out vec4 FragColor;

float n_plane = 0.1f;
float f_plane = 100.0f;

float linearizeDepth(float NDC)
{
    return (2*n_plane*f_plane) / (n_plane + f_plane - NDC * (f_plane - n_plane));
}

void main()
{
    float linear_Z = linearizeDepth(gl_FragCoord.z);
    FragColor = vec4(vec3(linear_Z)/f_plane,1.0);
}