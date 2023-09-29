#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

unsigned int startProgram, rectProgram, star1VAO, star2VAO, rectVAO;

glm::mat4 projection;
glm::mat4 model1(1.0f);
glm::mat4 model2(1.0f);
GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.0f };
GLfloat green[] = { 0.0f, 1.0f, 0.0f, 0.0f };

void createStarProgram() {
	const char* vertexShaderSource = "#version 330\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 Projection;\n"
		"uniform mat4 Model;\n"
		"uniform vec4 aColor;\n"
		"out vec4 Color;\n"
		"void main() {\n"
		"    gl_Position = Projection * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"    Color = aColor;\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330\n"
		"out vec4 FragColor;\n"
		"in vec4 Color;\n"
		"void main() {\n"
		"    FragColor = Color;\n"
		"}\0";


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

	startProgram = glCreateProgram();

	glAttachShader(startProgram, vertexShader);
	glAttachShader(startProgram, fragmentShader);
	glLinkProgram(startProgram);
	glGetProgramiv(startProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(startProgram, 512, NULL, log);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << log << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void createRectProgram() {
	const char* vsSource = "#version 330\n"
		"layout (location = 0) in vec2 aPos;\n"
		"out vec4 Color;"
		"uniform mat4 Projection;\n"
		"void main() {\n"
		"    gl_Position = Projection * vec4(aPos, 0.0, 1.0);\n"
		"}\0";
	const char* fsSource = "#version 330\n"
		"out vec4 FragColor;\n"
		"in vec4 Color;\n"
		"void main() {\n"
		"    FragColor = vec4(0.0, 0.0, 1.0, 0.0);\n"
		"}\0";
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vsSource, NULL);
	glShaderSource(fShader, 1, &fsSource, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

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

	rectProgram = glCreateProgram();

	glAttachShader(rectProgram, vShader);
	glAttachShader(rectProgram, fShader);
	glLinkProgram(rectProgram);
	glGetProgramiv(rectProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(rectProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

unsigned int createStar(int cx, int cy, int R, int r, int theta) {
	const float PI = glm::pi<float>();

	float rad = 2 * PI * theta / 360.f;  // 起始绘制角度

	float d = 2 * PI * 36.0f / 360.0f;  // 旋转增量 

	float long_r = R;  // 锐角顶点到中心距离

	float short_r = r;  // 非锐角顶点到中心距离

	float center[] = { (float)cx, (float)cy, 0.0f };

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

	unsigned int vao, vboHandler, eboHandler;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboHandler);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandler);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenBuffers(1, &eboHandler);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandler);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑
	glBindVertexArray(0);

	return vao;
}

void createRect(int x, int y, int width, int height) {
	float vertices[] = {
		(float)x,			(float)y,  // 左下角
		(float)x,			(float)(y + height),  // 左上角
		(float)(x + width), (float)(y + height), // 右上角
		(float)(x + width), (float)y // 右下角
	};

	unsigned int indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	unsigned int VBO, EBO;

	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(startProgram);

	unsigned int loc = glGetUniformLocation(startProgram, "Projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);
	loc = glGetUniformLocation(startProgram, "Model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &model1[0][0]);
	loc = glGetUniformLocation(startProgram, "aColor");
	glUniform4fv(loc, 1, red);

	glBindVertexArray(star1VAO);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glUseProgram(startProgram);
	loc = glGetUniformLocation(startProgram, "Model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &model2[0][0]);
	loc = glGetUniformLocation(startProgram, "aColor");
	glUniform4fv(loc, 1, green);
	glBindVertexArray(star2VAO);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glUseProgram(rectProgram);
	loc = glGetUniformLocation(rectProgram, "Projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(rectVAO);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (w > h)
		glViewport((w - h) / 2, 0, h, h);
	else
		glViewport(0, (h - w) / 2, w, w);

	int x = w > h ? h : w;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, x, 0, x);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho<float>(0, (float)x, 0, (float)x);
}


// 五角星1
const float localX = 100;  // 初始中心坐标 x
const float localY = 100;  // 初始中心坐标 y

float centerX = localX;  // 当前中心坐标 x
float centerY = localY;  // 当前中心坐标 y

float R = 50;  // 大圆半径
float r = R * 0.382;  // 小圆半径

float angle = 0;  // 旋转角
float alpha = 5;  // 旋转增量

float vx = 0.3f;  // x 方向速度
float vy = 0.05f;  // y 方向速度


// 五角星2
const float localX2 = 300;  // 初始中心坐标 x
const float localY2 = 300;  // 初始中心坐标 y

float centerX2 = localX2;  // 当前中心坐标 x
float centerY2 = localY2;  // 当前中心坐标 y

float R2 = 50;  // 大圆半径
float r2 = R2 * 0.382;  // 小圆半径

float angle2 = 0;  // 旋转角
float alpha2 = -5;  // 旋转增量
float scale2 = 1;  // 缩放量
float ds = 0.0001;  // 缩放量

float vx2 = -0.1f;  // x 方向速度
float vy2 = -0.2f;  // y 方向速度

float rectWidth = 500;
float rectHeight = 500;
float rectBottom = 50;
float rectLeft = 50;
float rectTop = rectBottom + rectHeight;
float rectRight = rectLeft + rectWidth;

void idle() {
	model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(centerX, centerY, 0.0f));
	model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	model1 = glm::translate(model1, glm::vec3(-localX, -localY, 0.0f));

	model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(centerX2, centerY2, 0.0f));
	model2 = glm::rotate(model2, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
	model2 = glm::scale(model2, glm::vec3(scale2, scale2, scale2));
	model2 = glm::translate(model2, glm::vec3(-localX2, -localY2, 0.0f));

	// 五角星 1 边界检测
	if (centerX + R >= rectRight) {
		vx = -vx;
		centerX = rectRight - R;
	} else if (centerX - R <= rectLeft) {
		vx = -vx;
		centerX = rectLeft + R;
	}
	if (centerY + R >= rectTop) {
		vy = -vy;
		centerY = rectTop - R;
	}
	else if (centerY - R <= rectBottom) {
		vy = -vy;
		centerY = rectBottom + R;
	}
	// 五角星 2 边界检测
	if (centerX2 + R2 * scale2 >= rectRight) {
		vx2 = -vx2;
		centerX2 = rectRight - R2 * scale2;
	}
	else if (centerX2 - R2 * scale2 <= rectLeft) {
		vx2 = -vx2;
		centerX2 = rectLeft + R2 * scale2;
	}
	if (centerY2 + R2 * scale2 >= rectTop) {
		vy2 = -vy2;
		centerY2 = rectTop - R2 * scale2;
	}
	else if (centerY2 - R2 * scale2 <= rectBottom) {
		vy2 = -vy2;
		centerY2 = rectBottom + R2 * scale2;
	}
	// 五角星彼此互相检测
	float d = glm::sqrt(glm::pow(centerX - centerX2, 2.0f) + glm::pow(centerY - centerY2, 2.0f));
	if (d < R + R2 * scale2 || ( d == R + R2 * scale2
		&& (vx * vx2 < 0 || vy * vy2 < 0))
		) {
		float vx1 = vx;
		float vy1 = vy;
		vx = vx2;
		vy = vy2;
		vx2 = vx1;
		vy2 = vy1;
	}

	centerX += vx;
	centerY += vy;
	angle += alpha;

	centerX2 += vx2;
	centerY2 += vy2;
	angle2 += alpha2;
	scale2 += ds;
	if (scale2 >= 2) {
		ds = -ds;
		scale2 = 2;
	}
	else if (scale2 <= 0.1) {
		scale2 = 0.1;
		ds = -ds;
	}

	glutPostRedisplay();
}

int main() {
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Hello, rolling star");

	glewInit();

	createStarProgram();
	createRectProgram();
	star1VAO = createStar(localX, localY, R, r, angle);
	star2VAO = createStar(localX2, localY2, R2, r2, angle2);

	createRect(rectLeft, rectBottom, rectWidth, rectHeight);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
