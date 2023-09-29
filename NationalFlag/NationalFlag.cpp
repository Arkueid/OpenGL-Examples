#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

// ��ʼ����
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

glm::mat4 projection;

unsigned int starShaderProgram, flagShaderProgram;

void createStarProgram() {
	unsigned int vShader, fShader;
	int success; char infoLog[512];

#pragma region StarShaderProgram
	starShaderProgram = glCreateProgram();

	const char* vsSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 ProjectionMatrix;\n"
		"void main() {\n"
		"    gl_Position = ProjectionMatrix * vec4(aPos, 1.0);\n"
		"}\0";
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	const char* fsSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(1.0f, 1.0f, 0.0f, 0.0f); //���������׼ͼ�������ȡ��ɫֵ\n"
		"}\0";
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILE_FAILED\n" << infoLog << std::endl;
	}

	glAttachShader(starShaderProgram, vShader);
	glAttachShader(starShaderProgram, fShader);
	glLinkProgram(starShaderProgram);

	glGetProgramiv(starShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(starShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
#pragma endregion

#pragma region FlagShaderProgram
	flagShaderProgram = glCreateProgram();
	vsSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 ProjectionMatrix;\n"
		"void main() {\n"
		"	gl_Position = ProjectionMatrix * vec4(aPos, 1.0);\n"
		"}\0";
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fsSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(0.93, 0.11, 0.15, 0.0); //���������׼ͼ������ȡ��ɫֵ\n"
		"}\0";
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << std::endl;
	}

	glAttachShader(flagShaderProgram, vShader);
	glAttachShader(flagShaderProgram, fShader);
	glLinkProgram(flagShaderProgram);

	glGetProgramiv(flagShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(flagShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
#pragma endregion

}

/**
* @brief	���������
* @param	cx		���� x ����
* @param	cy		���� y ����
* @param	R		��Բ�뾶
* @param	r		��Բ�뾶
* @param	theta	��ʼ��ת��
* @param	color	�����ɫ
*/
void createStar(int cx, int cy, int R, int r, int theta) {
	const float PI = glm::pi<float>();

	float rad = 2 * PI * theta / 360.f;  // ��ʼ���ƽǶ�

	float d = 2 * PI * 36.0f / 360.0f;  // ��ת���� 

	float long_r = R;  // ��Ƕ��㵽���ľ���

	float short_r = r;  // ����Ƕ��㵽���ľ���

	float center[] = { (float)cx, (float)cy, 0.0f };

	float points[3 * 11];

	// ���������
	points[30] = center[0];
	points[31] = center[1];
	points[32] = center[2];

	for (int i = 0; i < 30; i += 6) {
		// �������Ƕ���
		points[i] = points[30] + short_r * glm::cos<float>(rad);
		points[i + 1] = points[31] + short_r * glm::sin<float>(rad);
		points[i + 2] = 0.0f;
		rad += d;
		// ������Ƕ���
		points[i + 3] = points[30] + long_r * glm::cos<float>(rad);
		points[i + 4] = points[31] + long_r * glm::sin<float>(rad);
		points[i + 5] = 0.0f;
		rad += d;
	}

	// ���Դ�ӡ
	//int cnt = 0;
	//for (int i = 0; i < 30; i += 3, cnt++) {
	//	printf("Point%d(%f %f %f)\n", cnt, points[i], points[i + 1], points[i + 2]);
	//}

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

	unsigned int vaoHandler, vboHandler, eboHandler;

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // ���
	glBindVertexArray(0);

	glBindVertexArray(vaoHandler);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glDeleteBuffers(1, &vboHandler);
	glDeleteBuffers(1, &eboHandler);
	glDeleteVertexArrays(1, &vaoHandler);
}

/**
* @brief	���ƾ���
* @param	x		���½� x ����
* @param	y		���½� y ����
* @param	w		���ο��
* @param	h		���θ߶�
*/
void DragRectangle(int x, int y, int w, int h) {
	float xf = x;
	float yf = y;
	float wf = w;
	float hf = h;
	
	unsigned int vao, vbo;
	float points[] = {
		xf, yf, 0.0f,
		xf, yf + hf, 0.0f,
		xf + wf, yf, 0.0f,
		xf + wf, yf, 0.0f,
		xf, yf + hf, 0.0f,
		xf + wf, yf + hf, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glBindVertexArray(0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void display() {
	GLint loc;

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(flagShaderProgram);
	
	loc = glGetUniformLocation(flagShaderProgram, "ProjectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

	// ���������http://www.nmgfao.gov.cn/index.php?c=article&id=2988

	// ���쳤��� 3:2
	const int flagHeight = WINDOW_HEIGHT * 0.7;
	const int flagWidth = flagHeight * 3 / 2;
	const int flagLowerLeftX = (WINDOW_WIDTH - flagWidth) / 2;
	const int flagLowerLeftY = (WINDOW_HEIGHT - flagHeight) / 2;
	const int flagUpperLeftX = flagLowerLeftX;
	const int flagUpperLeftY = flagLowerLeftY + flagHeight;

	const int R1 = flagHeight * 3 / 10 / 2;
	const int r1 = R1 * 0.382f;  // �ƽ�ָ��
	const int R2 = flagHeight / 10 / 2;
	const int r2 = R2 * 0.382f;  // �ƽ�ָ��

	const int biggerStarCenterX = flagUpperLeftX + flagWidth / 2 / 3;
	const int biggerStartCenterY = flagUpperLeftY - flagHeight / 2 / 2;

	const int standardRad = 54;  // ��׼���ڷŵ�����ǳ�ʼ��ת�ǣ���λ�ȣ��� x ������Ϊ�ߣ�

	// �ĿŻ��Ƶ������
	int stars[4][2];
	stars[0][0] = flagUpperLeftX + flagWidth / 2 * 10 / 15;
	stars[0][1] = flagUpperLeftY - flagHeight / 2 * 2 / 10;

	stars[1][0] = flagUpperLeftX + flagWidth / 2 * 12 / 15;
	stars[1][1] = flagUpperLeftY - flagHeight / 2 * 4 / 10;

	stars[2][0] = flagUpperLeftX + flagWidth / 2 * 12 / 15;
	stars[2][1] = flagUpperLeftY - flagHeight / 2 * 7 / 10;

	stars[3][0] = flagUpperLeftX + flagWidth / 2 * 10 / 15;
	stars[3][1] = flagUpperLeftY - flagHeight / 2 * 9 / 10;

	// ���Ƶ��ĸ�����ǳ�ʼ��ת�ǣ��������εõ�
	int rads[4] = { 175, 152, 200, 177 };
	
	DragRectangle(flagLowerLeftX, flagLowerLeftY, flagWidth, flagHeight);

	glUseProgram(0);

	glUseProgram(starShaderProgram);

	loc = glGetUniformLocation(starShaderProgram, "ProjectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

	createStar(biggerStarCenterX, biggerStartCenterY, R1, r1, standardRad);

	for (int i = 0; i < 4; i++) {
		createStar(stars[i][0], stars[i][1], R2, r2, rads[i]);
	}

	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(int width, int height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
}

void keyboard(unsigned char key, int x, int y) {
	printf("Key down: %c\n", key);
	printf("Cursor point: (%d, %d)\n", x, y);
}

int main() {
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//˫���壬������ΪGLUT_SINGLE

	glutCreateWindow("Hello, national flag");

	glewInit();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	createStarProgram();

	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}