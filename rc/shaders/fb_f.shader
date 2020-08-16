#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{   
    // normal texture
    // fragColor = vec4(vec3(texture(texture1, TexCoord)), 1.0);

    // inverse
    fragColor = vec4(1.0 - vec3(texture(texture1, TexCoord)), 1.0);

    // grayscale
    // texColor = texture(texture1, TexCoord);
    // float avg = (texColor.r + texColor.g + texColor.b) / 3.0;
    // fragColor = vec4(avg, avg, avg, 1.0);
}