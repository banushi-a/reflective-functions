// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0)in vec3 position; 
layout(location=1)in vec3 color;

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 v_color;

void main()
{
    v_color = color;
    gl_Position = projectionMatrix * viewMatrix * modelTransformMatrix * vec4(position, 1.0);
}
// ==================================================================