#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D Tex;
uniform float texSOffset;
uniform float texTOffset;
void main() {
	FragColor = texture(Tex, TexCoords + vec2(texSOffset, texTOffset));
}