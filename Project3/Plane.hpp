#pragma once
#include "IDrawable.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

class Plane :
    IDrawable
{
public:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	unsigned int TextureID;
	cgProgram* Program;

	Plane();
	~Plane();

	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetTexture(unsigned int textureID);
	void SetProgram(cgProgram* program);
	cgProgram* GetProgram();
private:
	unsigned int vao;
	unsigned int vbo;
	int vertexCount;
	void createData();
};

