#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA
{
    vec3 Normal;
} data_in[];

out vec3 normal;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    normal = data_in[0].Normal;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position - 0.8f * vec4(data_in[0].Normal, 0.0f);
    normal = data_in[0].Normal;
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    normal = data_in[1].Normal;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - 0.8f * vec4(data_in[1].Normal, 0.0f);
    normal = data_in[1].Normal;
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[2].gl_Position;
    normal = data_in[2].Normal;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position - 0.8f * vec4(data_in[2].Normal, 0.0f);
    normal = data_in[2].Normal;
    EmitVertex();
    EndPrimitive();
}