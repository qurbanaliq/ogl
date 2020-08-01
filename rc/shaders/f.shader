#version 330
// light shader

out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	color = texture(texture1, TexCoords);
}