#include "Ring.hpp"

Ring::Ring(float r, float R)
{
	this->r = r;
	this->R = R;
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	TextureID = 0;
	Program = NULL;
	vao = vbo = 0;
	vertexCount = 0;
	createData();
}

Ring::~Ring()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteVertexArrays(1, &vao);
}

void Ring::Render()
{
	Program->SetUniform("model", ModelMatrix);
	Program->SetUniform("view", ViewMatrix);
	Program->SetUniform("projection", ProjectionMatrix);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount*3); 

	glBindVertexArray(0);
	glUseProgram(0);
}

void Ring::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Ring::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Ring::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Ring::SetProgram(cgProgram* program)
{
	Program = program;
}

void Ring::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}

cgProgram* Ring::GetProgram()
{
	return Program;
}

void Ring::createData()
{
	const int M = 40;  // 大圆离散个数
	const int N = 30;  // 截面离散半径

	int verticesIndex = 0;
	int normalsIndex = 0;
	vertexCount = M * N * 6;
	float* vertices = new float[vertexCount * 3];
	float* normals = new float[vertexCount * 3];

	// 截面旋转角
	float A1 = 0.0f;
	// 大圆旋转角
	float A2 = 0.0f;

	// 截面旋转角增量
	float da1 = 360.0f / N;
	// 大圆旋转角增量
	float da2 = 360.0f / M;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			float x11 = (this->R + this->r * glm::cos(glm::radians(A1))) * glm::cos(glm::radians(A2));
			float z11 = (this->R + this->r * glm::cos(glm::radians(A1))) * glm::sin(glm::radians(A2));

			float x21 = (this->R + this->r * glm::cos(glm::radians(A1 + da1))) * glm::cos(glm::radians(A2));
			float z21 = (this->R + this->r * glm::cos(glm::radians(A1 + da1))) * glm::sin(glm::radians(A2));

			float x12 = (this->R + this->r * glm::cos(glm::radians(A1))) * glm::cos(glm::radians(A2 + da2));
			float z12 = (this->R + this->r * glm::cos(glm::radians(A1))) * glm::sin(glm::radians(A2 + da2));

			float x22 = (this->R + this->r * glm::cos(glm::radians(A1 + da1))) * glm::cos(glm::radians(A2 + da2));
			float z22 = (this->R + this->r * glm::cos(glm::radians(A1 + da1))) * glm::sin(glm::radians(A2 + da2));

			// 圆心1
			float cx1 = this->R * glm::cos(glm::radians(A2));
			float cz1 = this->R * glm::sin(glm::radians(A2));
			// 圆心2
			float cx2 = this->R * glm::cos(glm::radians(A2 + da2));
			float cz2 = this->R * glm::sin(glm::radians(A2 + da2));

			// 定义两个三角形
			// 第一个三角形
			// 第一个顶点
			vertices[verticesIndex++] = x11;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1));
			vertices[verticesIndex++] = z11;
			normals[normalsIndex++] = x11 - cx1;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1));
			normals[normalsIndex++] = z11 - cz1;
			// 第二个顶点
			vertices[verticesIndex++] = x12;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1));
			vertices[verticesIndex++] = z12;
			normals[normalsIndex++] = x12 - cx2;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1));
			normals[normalsIndex++] = z12 - cz2;
			// 第三个顶点
			vertices[verticesIndex++] = x21;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			vertices[verticesIndex++] = z21;
			normals[normalsIndex++] = x21 - cx1;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			normals[normalsIndex++] = z21 - cz1;
			// 第二个三角形
			// 第一个顶点
			vertices[verticesIndex++] = x21;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			vertices[verticesIndex++] = z21;
			normals[normalsIndex++] = x21 - cx1;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			normals[normalsIndex++] = z21 - cz1;
			// 第二个顶点
			vertices[verticesIndex++] = x12;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1));
			vertices[verticesIndex++] = z12;
			normals[normalsIndex++] = x12 - cx2;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1));
			normals[normalsIndex++] = z12 - cz2;
			// 第三个顶点
			vertices[verticesIndex++] = x22;
			vertices[verticesIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			vertices[verticesIndex++] = z22;
			normals[normalsIndex++] = x22 - cx2;
			normals[normalsIndex++] = this->r * glm::sin(glm::radians(A1 + da1));
			normals[normalsIndex++] = z22 - cz2;

			A2 += da2;
		}
		A1 += da1;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertices;
	delete[] normals;
}
