#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TextCords;
    vec3 normal;
} gs_in[];

out vec2 textCords; 

uniform float time;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 TranslatePosition(vec4 pos,vec3 dir)
{
    vec3 translation = dir*(((sin(time) + 1.0) / 2.0))*1.0f;
    return pos + vec4(translation,0.0);
}

void main()
{
    vec3 normal = GetNormal();
    gl_Position = TranslatePosition(gl_in[0].gl_Position,normal);
    textCords = gs_in[0].TextCords;
    EmitVertex();

    gl_Position = TranslatePosition(gl_in[1].gl_Position,normal);
    textCords = gs_in[1].TextCords;
    EmitVertex();

    gl_Position = TranslatePosition(gl_in[2].gl_Position,normal);
    textCords = gs_in[2].TextCords;
    EmitVertex();

    EndPrimitive();
}