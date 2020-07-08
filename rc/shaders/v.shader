#version 330

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in vec2 aTexCoord;

out vec3 outColor;
out vec2 texCoord;

uniform float offset;

uniform mat4 transform;

void main(void)
{
	gl_Position = transform * vec4(aPos, 1.0f);
	outColor = aColor;
	texCoord = aTexCoord;
}