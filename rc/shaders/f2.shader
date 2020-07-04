#version 330

out vec4 color;
in vec3 outColor;
in vec4 newColor;

void main()
{
	color = newColor;
}