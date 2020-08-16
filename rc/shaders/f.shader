#version 330
// light shader

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, TexCoords);
}