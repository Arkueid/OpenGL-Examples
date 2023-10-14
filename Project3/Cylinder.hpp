#include "Drawable.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

// 离散化的底面圆周顶点个数
const int M = 100;

class Cylinder : Drawable
{
public:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	Cylinder(float r, float h);
	~Cylinder();

	float R, H;
	unsigned int ProgramID;
	unsigned int TopTextureID;  // 顶面材质
	unsigned int SideTextureID;  // 侧面材质

	void Render();
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetProgram(unsigned int programID);
	void SetTexture(unsigned int textureID);
	void SetTopTexture(unsigned int textureID);
	void SetSideTexture(unsigned int textureID);
private:
	unsigned int vao;
	unsigned int vbo;
	int vertexCount;
	void createData();
};