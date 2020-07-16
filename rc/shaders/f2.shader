#version 330
// object shader


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 color;

void main()
{	
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 texSpecular = vec3(texture(material.specular, texCoord));
	vec3 specular = light.specular * spec * texSpecular;
	
	// emission
	vec3 emission = vec3(0.0);
	if (texSpecular.r == 0.0)
		emission = vec3(texture(material.emission, texCoord));
	
	// final color
	vec3 result = ambient + diffuse + specular + emission;
	
	color = vec4(result, 1.0);	
}