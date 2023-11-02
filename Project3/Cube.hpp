#pragma once
#include "IDrawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

class Cube: IDrawable 
{
public:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	float A;  // ±ß³¤
	unsigned int TextureID;
	cgProgram* Program;
	float SOffset, TOffset;

	Cube(float a);
	~Cube();
	void SetPos(float x, float y, float z);
	void UpdateTexure(float sOffset, float tOffset);

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
	unsigned int vbo2;
	int vertexCount;
	void createData();
};