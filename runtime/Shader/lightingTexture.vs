#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
	Normal = transpose(inverse(mat3(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}