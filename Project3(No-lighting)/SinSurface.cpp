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
	const int M = 40;  // 旋转圆周离散化数量
	const int N = 40 * 4;

	vertexCount = M * N * 6;
	float* vertices = new float[vertexCount * 3];
	int verticesIndex = 0;

	float alpha = glm::radians(Alpha);  // 正弦相位
	float dalpha = glm::radians(Delta) / (float)N;  // 相位增量

	float k = 2 / 3;
	float x = -glm::radians(Delta) / 2;
	x *= k;
	float dx = dalpha;

	float angle = 0;  // 旋转角
	float dangle = 360.0f / M;  // 旋转角增量

	for (int i = 0; i < N; i++)
	{
		float R1 = glm::sin(alpha) + 1.5f;  // 旋转半径1
		float R2 = glm::sin(alpha + dalpha) + 1.5f;  // 邻近点旋转半径2
		// 旋转角置为0
		angle = 0;
		// 默认绕x轴旋转
		for (int j = 0; j < M; j++)
		{
			// 构造两个三角形
			// 第一个三角形
			// 第一个顶点
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle));
			// 第二个顶点
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle + dangle));
			// 第三个顶点
			vertices[verticesIndex++] = x + dx;
			vertices[verticesIndex++] = R2 * glm::sin(glm::radians(angle));
			vertices[verticesIndex++] = R2 * glm::cos(glm::radians(angle));

			// 第二个三角形
			// 第一个顶点
			vertices[verticesIndex++] = x + dx;
			vertices[verticesIndex++] = R2 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R2 * glm::cos(glm::radians(angle + dangle));
			// 第二个顶点
			vertices[verticesIndex++] = x;
			vertices[verticesIndex++] = R1 * glm::sin(glm::radians(angle + dangle));
			vertices[verticesIndex++] = R1 * glm::cos(glm::radians(angle + dangle));
			// 第三个顶点
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
