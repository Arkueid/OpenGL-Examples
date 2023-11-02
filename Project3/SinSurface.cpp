#include "SinSurface.hpp"

SinSurface::SinSurface(float phi, float delta)
{
	Phi = phi;
	Delta = delta;
	Program = NULL;
	TextureID = 0;
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
	Program->SetUniform("model", ModelMatrix);
	Program->SetUniform("view", ViewMatrix);
	Program->SetUniform("projection", ProjectionMatrix);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount + 20);

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

void SinSurface::SetProgram(cgProgram* program)
{
	Program = program;
}

void SinSurface::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}

cgProgram* SinSurface::GetProgram()
{
	return Program;
}

void SinSurface::createData()
{
	const int M = 40;  // 旋转圆周离散化数量
	const int N = 30 * 4;  // 正弦线离散量
	const float H = 1.5f;  // 正弦线y偏移量

	vertexCount = M * N * 6;
	float* vertices = new float[vertexCount * 3];
	float* normals = new float[vertexCount * 3];
	int verticesIndex = 0;
	int normalsIndex = 0;

	float x = 0.0f;
	float phi = Phi;
	float dphi = Delta / (float)N;  // 相位增量
	float alpha = 0.0f;  // 绕x轴旋转量
	float dalpha = 360.0f / (float)M;

	for (int i = 0; i < N; i++) 
	{
		float r1 = glm::sin(glm::radians(phi)) + H;
		float r2 = glm::sin(glm::radians(phi + dphi)) + H;

		alpha = 0.0f;

		for (int j = 0; j < M; j++)
		{
			// 第一个三角形
			vertices[verticesIndex++] = x; vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha)); vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			normals[normalsIndex++] = 2 * r1 * glm::cos(glm::radians(phi)); normals[normalsIndex++] = -r1 * glm::cos(glm::radians(alpha)); normals[normalsIndex++] = -r1 * glm::sin(glm::radians(alpha));

			vertices[verticesIndex++] = x; vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha)); vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = 2 * r1 * glm::cos(glm::radians(phi)); normals[normalsIndex++] = -r1 * glm::cos(glm::radians(alpha + dalpha)); normals[normalsIndex++] = -r1 * glm::sin(glm::radians(alpha + dalpha));

			vertices[verticesIndex++] = x + glm::radians(dphi); vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha)); vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			normals[normalsIndex++] = 2 * r2 * glm::cos(glm::radians(phi + dphi)); normals[normalsIndex++] = -r2 * glm::cos(glm::radians(alpha)); normals[normalsIndex++] = -r2 * glm::sin(glm::radians(alpha));
			
			// 第二个三角形
			vertices[verticesIndex++] = x + glm::radians(dphi); vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha)); vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			normals[normalsIndex++] = 2 * r2 * glm::cos(glm::radians(phi)); normals[normalsIndex++] = -r2 * glm::cos(glm::radians(alpha)); normals[normalsIndex++] = -r2 * glm::sin(glm::radians(alpha));

			vertices[verticesIndex++] = x + glm::radians(dphi); vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha)); vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = 2 * r2 * glm::cos(glm::radians(phi + dphi)); normals[normalsIndex++] = -r2 * glm::cos(glm::radians(alpha + dalpha)); normals[normalsIndex++] = -r2 * glm::sin(glm::radians(alpha + dalpha));

			vertices[verticesIndex++] = x; vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha)); vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = 2 * r1 * glm::cos(glm::radians(phi)); normals[normalsIndex++] = -r1 * glm::cos(glm::radians(alpha + dalpha)); normals[normalsIndex++] = -r1 * glm::sin(glm::radians(alpha + dalpha));

			alpha += dalpha;
		}

		x += glm::radians(dphi);
		phi += dphi;
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
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] normals;
}
