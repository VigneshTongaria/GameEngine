#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec2 TextCords;
    vec3 normal;
} gs_in[];

out vec2 textCords; 

uniform float time;


void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = (gl_in[index].gl_Position + vec4(gs_in[index].normal* 0.3,0.0));
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}