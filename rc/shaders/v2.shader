#version 330

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 fragPos;
out vec2 texCoord;

void main(void)
{
	
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fragPos = vec3(model * vec4(aPos, 1.0)); // to translate the fragments to world space
	// update normals with model
	Normal = mat3(transpose(inverse(model))) * aNormal; // for non-uniform scaling apply inverse and transpose
	texCoord = aTexCoord;
}