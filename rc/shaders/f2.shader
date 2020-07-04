#version 330

out vec4 color;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	color = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
	
}