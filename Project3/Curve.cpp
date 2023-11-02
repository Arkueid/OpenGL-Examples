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

// 计算n次贝塞尔曲线上的点
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

    constexpr float step = glm::pi<float>() / N; // 曲线步长参数

	const int M = 40;  // 旋转的圆周离散化个数

    vertexCount = N * M * 6;

    float* vertices = new float[vertexCount * 5];
	float* normals = new float[vertexCount * 3];
    int verticesIndex = 0;
	int normalsIndex = 0;

	float texX = 0.0f;  // 纹理坐标x
	float texY = 0.0f;  // 纹理坐标y
	float dtexX = 1.0f / M;  // 纹理x增量
	float dtexY = 1.0f / N;  // 纹理y增量

	float alpha = 0;  // 旋转角
	float dalpha = 360.0f / M;  // 角度增量

	float phi = 0.0f;
	
    for (int i = 0; i < N; i++) {
        Point p1 = curve(phi);
		Point p2 = curve(phi + step);
		// 未旋转的初始法向量1
		float dx1 = 2.0f * glm::sin(phi + glm::pi<float>());
		float dy1 = 1.0f;
		// 法向量1终点坐标
		float nx1 = p1.x + dx1;
		float ny1 = p1.y + dy1;
		// 未旋转的初始法向量2
		float dx2 = 2.0f * glm::sin(phi + step + glm::pi<float>());
		float dy2 = 1.0f;
		// 法向量2终点坐标
		float nx2 = p2.x + dx2;
		float ny2 = p2.y + dy2;
		// 在下方的点的半径
		float r1 = p1.x;
		// 在上方的点的半径
		float r2 = p2.x;
		alpha = 0.0f;
		// 将曲线上各点旋转一周（在此默认转轴为y轴
		for (int j = 0; j < M; j++)
		{
			// 定义两个三角形
			// 第一个三角形
			// 第一个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			// 纹理坐标
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// 第二个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY + dtexY;
			// 法向量
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha)) - r2 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha)) - r2 * glm::sin(glm::radians(alpha));
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;
			// 法向量
			normals[normalsIndex++] = nx2 * glm::cos(glm::radians(alpha + dalpha)) - r2 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny2 - p2.y;
			normals[normalsIndex++] = nx2 * glm::sin(glm::radians(alpha + dalpha)) - r2 * glm::sin(glm::radians(alpha + dalpha));
			// 第二个三角形
			// 第一个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha));
			vertices[verticesIndex++] = texX;
			vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha)) - r1 * glm::cos(glm::radians(alpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha)) - r1 * glm::sin(glm::radians(alpha));
			// 第二个顶点
			vertices[verticesIndex++] = r1 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p1.y;
			vertices[verticesIndex++] = r1 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY;
			// 法向量
			normals[normalsIndex++] = nx1 * glm::cos(glm::radians(alpha + dalpha)) - r1 * glm::cos(glm::radians(alpha + dalpha));
			normals[normalsIndex++] = ny1 - p1.y;
			normals[normalsIndex++] = nx1 * glm::sin(glm::radians(alpha + dalpha)) - r1 * glm::sin(glm::radians(alpha + dalpha));
			// 第三个顶点
			vertices[verticesIndex++] = r2 * glm::cos(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = p2.y;
			vertices[verticesIndex++] = r2 * glm::sin(glm::radians(alpha + dalpha));
			vertices[verticesIndex++] = texX + dtexX;
			vertices[verticesIndex++] = texY + dtexY;
			// 法向量
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
