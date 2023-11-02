#pragma once
#include "IDrawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

class SinSurface : IDrawable
{
public:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	unsigned int TextureID;
	cgProgram* Program;

	float Phi;  // ��ʼ�Ƕ�
	float Delta;  // ���

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="phi">��ʼ��λ</param>
	/// <param name="delta">��λ���</param>
	SinSurface(float phi, float delta);
	~SinSurface();

	// Drawable
	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetProgram(cgProgram* programID);
	void SetTexture(unsigned int textureID);
	cgProgram* GetProgram();
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int vbo2;
	int vertexCount;
	void createData();
};