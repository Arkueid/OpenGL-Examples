#include "Cylinder.hpp"

Cylinder::Cylinder(float r, float h)
{
	R = r;
	H = h;
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	createData();
}

Cylinder::~Cylinder()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Cylinder::Render()
{
	glUseProgram(ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TopTextureID);
	glUniform1i(glGetUniformLocation(ProgramID, "Tex"), 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount/2);  // ���Ƶ���

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SideTextureID);
	glUniform1i(glGetUniformLocation(ProgramID, "Tex"), 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, vertexCount / 2, vertexCount / 2);  // ���Ƶ���

	glBindVertexArray(0);
	glUseProgram(0);
}

void Cylinder::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Cylinder::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Cylinder::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Cylinder::SetProgram(unsigned int programID)
{
	ProgramID = programID;
}

// no use
void Cylinder::SetTexture(unsigned int textureID)
{
}

void Cylinder::SetTopTexture(unsigned int textureID)
{
	TopTextureID = textureID;
}

void Cylinder::SetSideTexture(unsigned int textureID)
{
	SideTextureID = textureID;
}

void Cylinder::createData()
{
	float y1 = -H / 2;  // �µ�������
	float y2 = H / 2;  // ��������

	float alpha = 0;  // ��ʼ�Ƕ�
	float dalpha = 360.f / M;  // �Ƕ�����
	
	// ������Ϣ
	vertexCount = 2 * M * 3 + M * 2 * 3;
	float *vertices = new float[vertexCount*5];

	int verticesIndex = 0;

	// �������
	for (int i = 0; i < 2; i++) {
		float y = i == 0 ? y1 : y2;
		for (int j = 0; j < M; j++)
		{
			// ����������
			// ��һ������
			float cos = glm::cos(glm::radians(alpha));
			float sin = glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = R * cos;
			vertices[verticesIndex++] = y;
			vertices[verticesIndex++] = R * sin;
			// ��������Բ�βü�
			vertices[verticesIndex++] = 0.5f * cos + 0.5f;
			vertices[verticesIndex++] = 0.5f * sin + 0.5f;

			// �ڶ�������
			cos = glm::cos(glm::radians(alpha + dalpha));
			sin = glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = R * cos;
			vertices[verticesIndex++] = y;
			vertices[verticesIndex++] = R * sin;
			// ��������Բ�βü�
			vertices[verticesIndex++] = 0.5f * cos + 0.5f;
			vertices[verticesIndex++] = 0.5f * sin + 0.5f;

			// ���������㣬��Բ�ģ�0, -H/2, 0��
			vertices[verticesIndex++] = 0;
			vertices[verticesIndex++] = y;
			vertices[verticesIndex++] = 0;
			// ��������Բ�βü�
			vertices[verticesIndex++] = 0.5f;
			vertices[verticesIndex++] = 0.5f;

			alpha += dalpha;
		}
	}

	alpha = 0.0f;
	float texX = 0.0f;
	float dtexX = 1.0f / M;  // ������������
	// �������
	for (int i = 0; i < M; i++)
	{
		float x1, z1, x2, z2;
		x1 = R * glm::cos(glm::radians(alpha));
		z1 = R * glm::sin(glm::radians(alpha));
		x2 = R * glm::cos(glm::radians(alpha + dalpha));
		z2 = R * glm::sin(glm::radians(alpha + dalpha));

		// �����һ��������
		// ��һ������
		vertices[verticesIndex++] = x1;
		vertices[verticesIndex++] = y1;
		vertices[verticesIndex++] = z1;
		// ��������
		vertices[verticesIndex++] = texX;
		vertices[verticesIndex++] = 0.0f;
		// �ڶ�������
		vertices[verticesIndex++] = x2;
		vertices[verticesIndex++] = y1;
		vertices[verticesIndex++] = z2;
		// ��������
		vertices[verticesIndex++] = texX + dtexX;
		vertices[verticesIndex++] = 0.0f;
		// ����������
		vertices[verticesIndex++] = x1;
		vertices[verticesIndex++] = y2;
		vertices[verticesIndex++] = z1;
		// ��������
		vertices[verticesIndex++] = texX;
		vertices[verticesIndex++] = 1.0f;

		// ����ڶ���������
		// ��һ������
		vertices[verticesIndex++] = x1;
		vertices[verticesIndex++] = y2;
		vertices[verticesIndex++] = z1;
		// ��������
		vertices[verticesIndex++] = texX;
		vertices[verticesIndex++] = 1.0f;
		// �ڶ�������
		vertices[verticesIndex++] = x2;
		vertices[verticesIndex++] = y2;
		vertices[verticesIndex++] = z2;
		// ��������
		vertices[verticesIndex++] = texX + dtexX;
		vertices[verticesIndex++] = 1.0f;
		// ����������
		vertices[verticesIndex++] = x2; vertices[verticesIndex++] = y1; vertices[verticesIndex++] = z2;
		// ��������
		vertices[verticesIndex++] = texX + dtexX;
		vertices[verticesIndex++] = 0.0f;

		alpha += dalpha;
		texX += dtexX;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 5 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] vertices;
}
