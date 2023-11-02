#include "SinSurface.hpp"

SinSurface::SinSurface(float alpha, float delta)
{
	Alpha = alpha;
	Delta = delta;
	ProgramID = TextureID = 0;
	ModelMatrix = ViewMatrix = ProjectionMatrix = glm::mat4(1.0f);
	vao = vbo = 0;
	vertexCount = 0;
	createData();
}

SinSurface::~SinSurface()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SinSurface::Render()
{
	glUseProgram(ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, TextureID);

	//glUniform1i(glGetUniformLocation(ProgramID, "Tex"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, vertexCount + 20);

	glBindVertexArray(0);
	glUseProgram(0);
}

void SinSurface::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void SinSurface::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void SinSurface::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void SinSurface::SetProgram(unsigned int programID)
{
	ProgramID = programID;
}

void SinSurface::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}

void SinSurface::createData()
{
	const int M = 40;  // ��תԲ����ɢ������
	const int N = 40 * 4;

	vertexCount = M * N * 6;
	float* vertices = new float[vertexCount * 3];
	int verticesIndex = 0;

	float alpha = glm::radians(Alpha);  // ������λ
	float dalpha = glm::radians(Delta) / (float)N;  // ��λ����

	float k = 2 / 3;
	float x = -glm::radians(Delta) / 2;
	x *= k;
	float dx = dalpha;

	float angle = 0;  // ��ת��
	float dangle = 360.0f / M;  // ��ת������

	for (int i = 0; i < N; i++)
	{
		float R1 = glm::sin(alpha) + 1.5f;  // ��ת�뾶1
		float R2 = glm::sin(alpha + dalpha) + 1.5f;  // �ڽ�����ת�뾶2
		// ��ת����Ϊ0
		angle = 0;
		// Ĭ����x����ת
		for (int j = 0; j < M; j++)
		{
			// ��������������
			// ��һ��������
			// ��һ������
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle));
			// �ڶ�������
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle + dangle));
			// ����������
			vertices[verticesIndex++] = x + dx;
			vertices[verticesIndex++] = R2 * glm::sin(glm::radians(angle));
			vertices[verticesIndex++] = R2 * glm::cos(glm::radians(angle));

			// �ڶ���������
			// ��һ������
			vertices[verticesIndex++] = x + dx;
			vertices[verticesIndex++] = R2 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R2 * glm::cos(glm::radians(angle + dangle));
			// �ڶ�������
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle + dangle));
			// ����������
			vertices[verticesIndex++] = x + dx;
			vertices[verticesIndex++] = R2 * glm::sin(glm::radians(angle));
			vertices[verticesIndex++] = R2 * glm::cos(glm::radians(angle));

			angle += dangle;
		}
		alpha += dalpha;
		x += dx;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}
