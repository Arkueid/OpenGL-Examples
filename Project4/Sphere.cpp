#include "Sphere.hpp"

Sphere::Sphere(float r) 
{
	R = r;
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	createData();
}

Sphere::~Sphere() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Sphere::createData() 
{
	const int M = 40;  // 垂直方向离散点个数
	const int N = 40;  // 水平方向离散点个数
	const int size = M * N * 6;

	// 用柱坐标表示球面上的顶点
	float phi = -90.0f;  // 与xoz平面初始夹角 [-90, 90]
	float theta = 0.0f;  // 与x轴初始夹角 [0, 360]
	const float dphi = 180.0f / M;  // 垂直方向phi增量
	const float dtheta = 360.0f / N;  // 水平方向theta增量

	// 顶点坐标
	dataSize = size;
	int vertexIndex = 0;
	float* vertices = new float[size * 3];

	float x = 0.0f; // 纹理坐标x
	float y = 0.0f; // 纹理坐标y
	float dx = 1.0f / M; // 纹理坐标x增量
	float dy = 1.0f / N; // 纹理坐标y增量

	// 纹理坐标
	int texIndex = 0;
	float* texCoords = new float[size * 2];

	// 法线
	int normalIndex = 0;
	float* normals = new float[size * 3];
	
	for (int i = 0; i < M; i++) 
	{
		float z1 = R * glm::sin(glm::radians(phi));
		float z2 = R * glm::sin(glm::radians(phi + dphi));

		for (int j = 0; j < N; j++)
		{
			// 第一个顶点
			float x11 = R * glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta));
			float y11 = R * glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta));

			// 第二个顶点
			float x12 = R * glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta + dtheta));
			float y12 = R * glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta + dtheta));

			// 第三个顶点
			float x21 = R * glm::cos(glm::radians(phi + dphi)) * glm::cos(glm::radians(theta));
			float y21 = R * glm::cos(glm::radians(phi + dphi)) * glm::sin(glm::radians(theta));

			// 第四个顶点
			float x22 = R * glm::cos(glm::radians(phi + dphi)) * glm::cos(glm::radians(theta + dtheta));
			float y22 = R * glm::cos(glm::radians(phi + dphi)) * glm::sin(glm::radians(theta + dtheta));

			// 存入三角形数据
			// 第一个三角形
			vertices[vertexIndex++] = x11; vertices[vertexIndex++] = y11; vertices[vertexIndex++] = z1;
			texCoords[texIndex++] = x; texCoords[texIndex++] = y;
			normals[normalIndex++] = x11; normals[normalIndex++] = y11; normals[normalIndex++] = z1;

			vertices[vertexIndex++] = x12; vertices[vertexIndex++] = y12; vertices[vertexIndex++] = z1;
			texCoords[texIndex++] = x + dx; texCoords[texIndex++] = y;
			normals[normalIndex++] = x12; normals[normalIndex++] = y12; normals[normalIndex++] = z1;

			vertices[vertexIndex++] = x21; vertices[vertexIndex++] = y21; vertices[vertexIndex++] = z2;
			texCoords[texIndex++] = x; texCoords[texIndex++] = y + dy;
			normals[normalIndex++] = x21; normals[normalIndex++] = y21; normals[normalIndex++] = z2;

			// 第二个三角形
			vertices[vertexIndex++] = x22; vertices[vertexIndex++] = y22; vertices[vertexIndex++] = z2;
			texCoords[texIndex++] = x + dx; texCoords[texIndex++] = y + dy;
			normals[normalIndex++] = x22; normals[normalIndex++] = y22; normals[normalIndex++] = z2;

			vertices[vertexIndex++] = x12; vertices[vertexIndex++] = y12; vertices[vertexIndex++] = z1;
			texCoords[texIndex++] = x + dx; texCoords[texIndex++] = y;
			normals[normalIndex++] = x12; normals[normalIndex++] = y12; normals[normalIndex++] = z1;

			vertices[vertexIndex++] = x21; vertices[vertexIndex++] = y21; vertices[vertexIndex++] = z2;
			texCoords[texIndex++] = x; texCoords[texIndex++] = y + dy;
			normals[normalIndex++] = x21; normals[normalIndex++] = y21; normals[normalIndex++] = z2;

			theta += dtheta;
			x += dx;
		}
		y += dy;
		phi += dphi;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(float), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vbo3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertices;
	delete[] texCoords;
	delete[] normals;
}

void Sphere::SetPos(float x, float y, float z)
{
	ModelMatrix *= glm::translate(ModelMatrix, glm::vec3(x, y, z));
}

void Sphere::Render() 
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glUniform1i(glGetUniformLocation(ProgramID, "sampler"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, dataSize);

	glBindVertexArray(0);
}
void Sphere::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Sphere::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Sphere::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Sphere::SetProgram(unsigned int programID)
{
	ProgramID = programID;
}

void Sphere::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}
