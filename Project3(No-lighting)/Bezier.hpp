#pragma once
#include "Drawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>

struct Point
{
	float x, y, z;
};

class Bezier : Drawable
{
public:
	std::vector<Point> Points;  // ¿ØÖÆµã
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	unsigned int TextureID;
	unsigned int ProgramID;

	Bezier();
	Bezier(std::vector<Point>& points);
	~Bezier();
	void SetPoints(std::vector<Point>& points);

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
	Point bezier_curve(float t);
};
