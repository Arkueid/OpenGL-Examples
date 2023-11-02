#pragma once
#include "Drawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

class Ring : Drawable
{
public:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	unsigned int TextureID;
	unsigned int ProgramID;

	float r;
	float R;

	Ring(float r, float R);
	~Ring();

	// Drawable
	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetProgram(unsigned int programID);
	void SetTexture(unsigned int textureID);
private:
	unsigned int vao;
	unsigned int vbo;
	int vertexCount;
	void createData();
};