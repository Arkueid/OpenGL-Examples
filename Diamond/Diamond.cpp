#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

unsigned int VAO, startProgram;

void createProgram() {
	const char* vsSource = "#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"void main() {\n"
		"    gl_Position = vec4(aPos, 0.0, 1.0);\n"
		"}\0";
	const char* fsSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(0.0, 0.0, 1.0, 0.0);\n"
		"}\0";
	unsigned int vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glShaderSource(fShader, 1, &fsSource, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	startProgram = glCreateProgram();
	glAttachShader(startProgram, vShader);
	glAttachShader(startProgram, fShader);
	glLinkProgram(startProgram);

	int success; char infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetProgramiv(startProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(startProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

int totalPoint;

void DrawDiamond() {
	const float PI = glm::pi<float>();
	const int circlePoint = 30;
	const int circleCount = 1;
	const float dr = 0.9f / circleCount;
	const float x0 = dr;
	const float y0 = 0.0f;
	const float da = 2.0f * PI / circlePoint;
	totalPoint = circleCount * circlePoint;

	float* points = new float[circleCount * circlePoint * 2];
	unsigned int* indices = new unsigned int[totalPoint * (totalPoint - 1)];

	for (int i = 0; i < circleCount; i++) {
		for (int j = 0; j < circlePoint; j++) {
			int idx = i * circlePoint * 2 + j * 2;
			float angle = j * da;
			float R = (i + 1) * dr;
			points[idx] = R * glm::cos(angle);
			points[idx + 1] = R * glm::sin(angle);
		}
	}
	int cnt = 0;
	for (int i = 0; i < totalPoint; i++) {
		for (int j = i+1; j < totalPoint; j++) {
			indices[cnt++] = i;
			indices[cnt++] = j;
		}
	}

	printf("%d\n", cnt/2);

	unsigned int vbo, ebo;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, totalPoint * 2 * sizeof(float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalPoint * (totalPoint - 1) * sizeof(int), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] points;
	delete[] indices;
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(startProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, totalPoint * (totalPoint - 1), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (w >= h)
		glViewport((w - h) / 2, 0, h, h);
	else
		glViewport(0, (h - w) / 2, w, w);
}

int main() {
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(600, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello, diamond");

	glewInit();

	createProgram();

	DrawDiamond();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}