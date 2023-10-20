#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 ProjectionMatrix;
void main() {
	gl_Position = ProjectionMatrix * vec4(aPos, 1.0);
}