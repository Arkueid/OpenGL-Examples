#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

unsigned int vaoHandler;
unsigned int vboHandler;
unsigned int eboHandler;
unsigned int shaderProgram;

//#define FILL_DIFF

void initFivePointedStar() {
	const float PI = glm::pi<float>();

	float rad = 2 * PI / 5;  // 起始绘制角度

	rad += PI * 36.0f / 180.0f;

	float d = 2 * PI * 36.0f / 360.0f;  // 旋转增量 

	float long_r = 1.0f;  // 锐角顶点到中心距离

	float short_r = 0.38f;  // 非锐角顶点到中心距离

	float center[] = { 0.0f, 0.0f, 0.0f };

#ifdef FILL_DIFF
	float points[3 * 3 * 10];
	float color[3 * 3 * 10];
	float fillColor1[] = {1.0f, 0.0f, 0.0f};
	float fillColor2[] = { 0.5f, 0.0f, 0.0f };

	// 每次循环计算五角星的一个角所需参数（即两个三角形，六个顶点及其颜色）
	for (int i = 0; i < 90; i += 18) {
		// 计算非锐角顶点1
		points[i] = center[0] + short_r * glm::cos<float>(rad);
		points[i + 1] = center[1] + short_r * glm::sin<float>(rad);
		points[i + 2] = 0.0f;
		rad += d;
		// 计算两个三角形共用的锐角顶点
		points[i + 9 + 3] = points[i + 3] = center[0] + long_r * glm::cos<float>(rad);
		points[i + 9 + 4] = points[i + 4] = center[1] + long_r * glm::sin<float>(rad);
		points[i + 9 + 5] = points[i + 5] = 0.0f;
		rad += d;
		// 设置两个三角形共用的五角星中心
		points[i + 9 + 6] = points[i + 6] = center[0];
		points[i + 9 + 7] = points[i + 7] = center[1];
		points[i + 9 + 8] = points[i + 8] = 0.0f;
		
		// 计算非锐角顶点2
		points[i + 9] = center[0] + short_r * glm::cos<float>(rad);
		points[i + 9 + 1] = center[1] + short_r * glm::sin<float>(rad);
		points[i + 9 + 2] = 0.0f;

		// 设置差分颜色
		for (int j = i; j < i + 9; j += 3) {
			color[j] = fillColor1[0];
			color[j + 1] = fillColor1[1];
			color[j + 2] = fillColor1[2];
		}
		
		for (int j = i+9; j < i + 18; j += 3) {
			color[j] = fillColor2[0];
			color[j + 1] = fillColor2[1];
			color[j + 2] = fillColor2[2];
		}
	}

	unsigned int vertexVBO, colorVBO;
	glGenVertexArrays(1, &vaoHandler);
	glGenBuffers(1, &vertexVBO);
	glGenBuffers(1, &colorVBO);
	glBindVertexArray(vaoHandler);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
#else
	float points[3 * 11];
	
	// 五角星中心
	points[30] = center[0];
	points[31] = center[1];
	points[32] = center[2];  

	for (int i = 0; i < 30; i += 6) {
		// 计算非锐角顶点
		points[i] = points[30] + short_r * glm::cos<float>(rad);
		points[i + 1] = points[31] + short_r * glm::sin<float>(rad);
		points[i + 2] = 0.0f;
		rad += d;
		// 计算锐角顶点
		points[i + 3] = points[30] + long_r * glm::cos<float>(rad);
		points[i + 4] = points[31] + long_r * glm::sin<float>(rad);
		points[i + 5] = 0.0f;
		rad += d;
	}
	// 测试打印
	//int cnt = 0;
	//for (int i = 0; i < 30; i += 3, cnt++) {
	//	printf("Point%d(%f %f %f)\n", cnt, points[i], points[i + 1], points[i + 2]);
	//}

#ifdef FILL_UNIFORM
	unsigned int indices[] = {
		0, 1, 10,
		1, 2, 10,
		2, 3, 10,
		3, 4, 10,
		4, 5, 10,
		5, 6, 10,
		6, 7, 10,
		7, 8, 10,
		8, 9, 10,
		9, 0, 10
	};
#else
	unsigned int indices[] = {
		0, 1, 1, 2,
		2, 3, 3, 4,
		4, 5, 5, 6,
		6, 7, 7, 8,
		8, 9, 9, 0
	};
#endif // FILL_UNIFORM

	glGenVertexArrays(1, &vaoHandler);
	glBindVertexArray(vaoHandler);

	glGenBuffers(1, &vboHandler);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandler);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &eboHandler);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandler);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
#endif // FILL_DIFF
}

void createShaderProgram() {
	const char* vertexShaderSource = "#version 330\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 Color;"
		"void main() {\n"
		"    Color = aColor;"
		"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

#ifdef FILL_DIFF
	const char* fragmentShaderSource = "#version 330\n"
		"in vec3 Color;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(Color, 1.0f);\n"
		"}\0";
#else
	const char* fragmentShaderSource = "#version 330\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";
#endif // FILL_DIFF

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	}

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
	}

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << log << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void reshape(int width, int height) {
	glViewport(width / 2 - 250, height / 2 - 250, 500, 500);
}

void keyboard(unsigned char key, int x, int y) {
	printf("Key down: %c\n", key);
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(vaoHandler);

#ifdef FILL_UNIFORM
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
#elif FILL_DIFF 1
	glDrawArrays(GL_TRIANGLES, 0, 30);
#else
	glDrawElements(GL_LINE_STRIP, 20, GL_UNSIGNED_INT, 0);
#endif // FILL


	glutSwapBuffers();
}

int main() {
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(500, 500);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Hello, five-pointed star");

	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	createShaderProgram(); 

	initFivePointedStar();

	glUseProgram(shaderProgram);
	
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}