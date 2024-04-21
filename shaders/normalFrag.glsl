#version 330 core

out vec4 FragColor;

in vec3 normal;

void main()
{
	FragColor = vec4(-1 * normal, 1.00f);
}