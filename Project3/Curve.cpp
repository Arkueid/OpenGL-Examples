#include "Curve.hpp"

Curve::Curve()
{
	ProjectionMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::mat4(1.0f);
	vao = vbo = -1;
	TextureID = 0;
	vertexCount = 0;
	Program = NULL;
	createData();
}

Curve::~Curve()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Curve::Render()
{
	Program->SetUniform("model", ModelMatrix);
	Program->SetUniform("view", ViewMatrix);
	Program->SetUniform("projection", ProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount+20);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Curve::SetModelMatrix(glm::mat4 model)
{
	ModelMatrix = model;
}

void Curve::SetViewMatrix(glm::mat4 view)
{
	ViewMatrix = view;
}

void Curve::SetProjectionMatrix(glm::mat4 projection)
{
	ProjectionMatrix = projection;
}

void Curve::SetProgram(cgProgram* program)
{
	Program = program;
}

cgProgram* Curve::GetProgram()
{
	return Program;
}
void Curve::SetTexture(unsigned int textureID)
{
	TextureID = textureID;
}

// ����n�α����������ϵĵ�
Point Curve::curve(float t) {
	Point res;
	res.x = t;
	res.y = 2.0f * glm::cos(t + glm::pi<float>()) + 2.0f;
	res.z = 0.0f;
    return res;
}

void Curve::createData()
{
	const int N = 30;

    constexpr float step = glm::pi<float>() / N; // ���߲�������

	const int M = 40;  // ��ת��Բ����ɢ������

    vertexCount = N * M * 6;

    float* vertices = new float[vertexCount * 5];
	float* normals = new float[vertexCount * 3];
    int verticesIndex = 0;
	int normalsIndex = 0;

	float texX = 0.0f;  // ��������x
	float texY = 0.0f;  // ��������y
	float dtexX = 1.0f / M;  // ����x����
	float dtexY = 1.0f / N;  // ����y����

	float alpha = 0;  // ��ת��
	float dalpha = 360.0f / M;  // �Ƕ�����

	float phi = 0.0f;
	
    for (int i = 0; i < N; i++) {
        Point p1 = curve(phi);
		Point p2 = curve(phi + step);
		// δ��ת�ĳ�ʼ������1
		float dx1 = 2.0f * glm::sin(phi + glm::pi<float>());
		float dy1 = 1.0f;
		// ������1�յ�����
		float nx1 = p1.x + dx1;
		float ny1 = p1.y + dy1;
		// δ��ת�ĳ�ʼ������2
		float dx2 = 2.0f * glm::sin(phi + step + glm::pi<float>());
		float dy2 = 1.0f;
		// ������2�յ�����
		float nx2 = p2.x + dx2;
		float ny2 = p2.y + dy2;
		// ���·��ĵ�İ뾶
		float r1 = p1.x;
		// ���Ϸ��ĵ�İ뾶
		float r2 = p2.x;
		alpha = 0.0f;
		// �������ϸ�����תһ�ܣ��ڴ�Ĭ��ת��Ϊy��
		for (int j = 0; j < M; j++)
		{
			// ��������������
			// ��һ��������
			// ��һ������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			// ��������
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// �ڶ�������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha)) - r2 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha)) - r2 * glm::sin(glm::radians(alpha));
			// ����������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));
			// �ڶ���������
			// ��һ������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// �ڶ�������
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY;
			// ������
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha + dalpha)) - r1 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha + dalpha)) - r1 * glm::sin(glm::radians(alpha + dalpha));
			// ����������
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;
			// ������
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));

			alpha += dalpha;
			texX += dtexX;
		}
		texY += dtexY;
		phi += step;
    }

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo2);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 5 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] normals;
}
