#include "Cube.hpp"

Cube::Cube(float a) 
{
	A = a;
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	createData();
}

void Cube::createData()
{
	float a = A / 2;
	const int size = 6 * 2 * 3 * 5;
	vertexCount = 6 * 2 * 3;
	float vertices[size] = {
		// 后
		-a, -a, -a,  0.0f, 0.0f,
		 a, -a, -a,  0.2f, 0.0f,
		 a,  a, -a,  0.2f, 0.2f,
		 a,  a, -a,  0.2f, 0.2f,
		-a,  a, -a,  0.0f, 0.2f,
		-a, -a, -a,  0.0f, 0.0f,

		// 前
		-a, -a,  a,  0.0f, 0.0f,
		 a, -a,  a,  0.2f, 0.0f,
		 a,  a,  a,  0.2f, 0.2f,
		 a,  a,  a,  0.2f, 0.2f,
		-a,  a,  a,  0.0f, 0.2f,
		-a, -a,  a,  0.0f, 0.0f,

		// 左
		-a,  a,  a,  0.2f, 0.0f,
		-a,  a, -a,  0.2f, 0.2f,
		-a, -a, -a,  0.0f, 0.2f,
		-a, -a, -a,  0.0f, 0.2f,
		-a, -a,  a,  0.0f, 0.0f,
		-a,  a,  a,  0.2f, 0.0f,

		// 右
		 a,  a,  a,  0.2f, 0.0f,
		 a,  a, -a,  0.2f, 0.2f,
		 a, -a, -a,  0.0f, 0.2f,
		 a, -a, -a,  0.0f, 0.2f,
		 a, -a,  a,  0.0f, 0.0f,
		 a,  a,  a,  0.2f, 0.0f,

		 // 下
		-a, -a, -a,  0.0f, 0.2f,
		 a, -a, -a,  0.2f, 0.2f,
		 a, -a,  a,  0.2f, 0.0f,
		 a, -a,  a,  0.2f, 0.0f,
		-a, -a,  a,  0.0f, 0.0f,
		-a, -a, -a,  0.0f, 0.2f,

		// 上
		-a,  a, -a,  0.0f, 0.2f,
		 a,  a, -a,  0.2f, 0.2f,
		 a,  a,  a,  0.2f, 0.0f,
		 a,  a,  a,  0.2f, 0.0f,
		-a,  a,  a,  0.0f, 0.0f,
		-a,  a, -a,  0.0f, 0.2f
	};

	float normals[6*2*3*3] = {
		// 后
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		
		// 前
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,

		// 左
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		// 右
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		// 上
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		// 下
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Cube::~Cube() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Cube::SetPos(float x, float y, float z)
{
	ModelMatrix *= glm::translate(ModelMatrix, glm::vec3(x, y, z));
}

void Cube::UpdateTexure(float sOffset, float tOffset)
{
	SOffset = sOffset;
	TOffset = tOffset;
}

void Cube::Render()
{
	Program->SetUniform("model", ModelMatrix);
	Program->SetUniform("view", ViewMatrix);
	Program->SetUniform("projection", ProjectionMatrix);
	Program->SetUniform("texSOffset", SOffset);
	Program->SetUniform("texTOffset", TOffset);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Cube::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Cube::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Cube::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Cube::SetProgram(cgProgram* program)
{
	Program = program;
}

void Cube::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}

cgProgram* Cube::GetProgram()
{
	return Program;
}