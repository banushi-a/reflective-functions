#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Send to the Fragment Shader
out vec2 v_texCoord;
out vec3 Normal;

in DATA
{
    vec2 v_texCoord;
    vec3 Normal;
} data_in[];

void main()
{
    gl_Position = gl_in[0].gl_Position;
    Normal = data_in[0].Normal;
    v_texCoord = data_in[0].v_texCoord;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    Normal = data_in[1].Normal;
    v_texCoord = data_in[1].v_texCoord;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    Normal = data_in[2].Normal;
    v_texCoord = data_in[2].v_texCoord;
    EmitVertex();

    EndPrimitive();
}