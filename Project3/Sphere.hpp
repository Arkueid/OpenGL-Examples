#pragma once
#include "IDrawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

class Sphere: IDrawable {
public:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;

	float R;
	cgProgram* Program;
	unsigned int TextureID;

	Sphere(float r);
	~Sphere();
	void SetPos(float x, float y, float z);
	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetProgram(cgProgram* program);
	cgProgram* GetProgram();
	void SetTexture(unsigned int textureID);
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int vbo2;
	unsigned int vbo3;
	int dataSize;
	void createData();
};