#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

struct Material
{
    sampler2D diffuse1;
    sampler2D specular1;
};

uniform Material material;
uniform vec3 viewDir;

void main()
{ 
    vec3 lightDirection = normalize(vec3(0.2, 0.1, -0.5));
    float diff = max(dot(-lightDirection, Normal), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse1, TexCoords));

    vec3 reflectDir = reflect(lightDirection, Normal);
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * vec3(texture(material.specular1, TexCoords));

    vec3 ambient = 0.1 * vec3(texture(material.diffuse1, TexCoords));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}