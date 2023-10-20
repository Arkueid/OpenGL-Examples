#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D Tex;
uniform float texSOffset;
uniform float texTOffset;
void main() {
	FragColor = texture(Tex, TexCoord + vec2(texSOffset, texTOffset));
}