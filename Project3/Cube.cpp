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
		-a, -a, -a,  0.0f, 0.0f,
		 a, -a, -a,  1.0f, 0.0f,
		 a,  a, -a,  1.0f, 1.0f,
		 a,  a, -a,  1.0f, 1.0f,
		-a,  a, -a,  0.0f, 1.0f,
		-a, -a, -a,  0.0f, 0.0f,

		-a, -a,  a,  0.0f, 0.0f,
		 a, -a,  a,  1.0f, 0.0f,
		 a,  a,  a,  1.0f, 1.0f,
		 a,  a,  a,  1.0f, 1.0f,
		-a,  a,  a,  0.0f, 1.0f,
		-a, -a,  a,  0.0f, 0.0f,

		-a,  a,  a,  1.0f, 0.0f,
		-a,  a, -a,  1.0f, 1.0f,
		-a, -a, -a,  0.0f, 1.0f,
		-a, -a, -a,  0.0f, 1.0f,
		-a, -a,  a,  0.0f, 0.0f,
		-a,  a,  a,  1.0f, 0.0f,

		 a,  a,  a,  1.0f, 0.0f,
		 a,  a, -a,  1.0f, 1.0f,
		 a, -a, -a,  0.0f, 1.0f,
		 a, -a, -a,  0.0f, 1.0f,
		 a, -a,  a,  0.0f, 0.0f,
		 a,  a,  a,  1.0f, 0.0f,

		-a, -a, -a,  0.0f, 1.0f,
		 a, -a, -a,  1.0f, 1.0f,
		 a, -a,  a,  1.0f, 0.0f,
		 a, -a,  a,  1.0f, 0.0f,
		-a, -a,  a,  0.0f, 0.0f,
		-a, -a, -a,  0.0f, 1.0f,

		-a,  a, -a,  0.0f, 1.0f,
		 a,  a, -a,  1.0f, 1.0f,
		 a,  a,  a,  1.0f, 0.0f,
		 a,  a,  a,  1.0f, 0.0f,
		-a,  a,  a,  0.0f, 0.0f,
		-a,  a, -a,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

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

void Cube::Render()
{
	glUseProgram(ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glUniform1i(glGetUniformLocation(ProgramID, "Tex"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12*3);

	glBindVertexArray(0);
	glUseProgram(0);
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

void Cube::SetProgram(unsigned int programID)
{
	ProgramID = programID;
}

void Cube::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}
