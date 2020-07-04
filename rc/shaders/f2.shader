#version 330

out vec4 color;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D outTexture;

void main()
{
	color = texture(outTexture, texCoord);
	
}