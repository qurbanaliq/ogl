#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 aColor;

out vec3 outColor;
out vec4 newColor;

uniform float offset;

void main(void)
{
	gl_Position = vec4(offset + position.x, position.y * -1, position.z, 1.0);
	outColor = aColor;
	newColor = vec4(position, 1.0);
}