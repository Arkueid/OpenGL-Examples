#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/glew.h>
#include <GL/glut.h>

unsigned int program, texture;
unsigned int VBO, VAO, EBO;

void createShaderProgram() {
	unsigned int vShader, fShader;
	const char* vsSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"    TexCoord = aTexCoord;\n"
		"}\n";
	const char* fsSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"    FragColor = texture(ourTexture, TexCoord);\n"
		"}\n";
	program = glCreateProgram();
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glShaderSource(fShader, 1, &fsSource, NULL);

	int success; char infoLog[512];
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

int totalPoints;
float startAngle = 0.0f;

void Draw() {
	/**
	* ����һ�� 15 * 10 �Ķ�������
	* ͨ�� z ������ı任��ʵ��ӭ��Ʈ���Ĳ��˸�
	* x1, y1, z1, tx1, ty1, x2, y2, z2, tx2, ty2 ...
	* һ������ vertex = (x: x����, y: y����, z: z����, tx: ���������x, ty: ���������y)
	* z = sin(A + dn) A Ϊ��һ�������λ��dΪ������nΪ��ǰ�������
	* d = Ԥ�ȶ����������λ��� / ������
	* tx = x
	* ty = y
	*/

	const int W = 150, H = 100;
	float points[(W + H) * 2 * 5];
	float upperLeftX = -0.8f;
	float upperLeftY = 0.8f;
	float xSpan = 1.6f;
	float ySpan = 1.6f;
	float dx = xSpan / W;
	float dy = ySpan / H;

	// ���Ҳ���ز���
	const float PI = glm::pi<float>();
	float angleSpan = 4 * PI;  // ��λ���
	float deltaA = angleSpan / W;  // ���������λ����
	float heightSpan = 0.04;  // ���

	totalPoints = (W + H) * 2;

	for (int i = 0; i < W; i++) {
		// ��
		points[i * 5 + 0] = upperLeftX + i * dx;
		points[i * 5 + 1] = upperLeftY + glm::sin<float>(startAngle + i * deltaA) * heightSpan;
		points[i * 5 + 2] = 0.0f;
		points[i * 5 + 3] = ((float)i) / ((float)W);
		points[i * 5 + 4] = 0.0f;
		// ��
		int dn = (i + W + H) * 5;
		points[dn + 0] = upperLeftX + xSpan - i * dx;
		points[dn + 1] = upperLeftY - ySpan + glm::sin<float>(startAngle + (W - i) * deltaA) * heightSpan;
		points[dn + 2] = 0.0f;
		points[dn + 3] = 1.0f - ((float)i) / ((float)W);
		points[dn + 4] = 1.0f;
	}

	for (int i = 0; i < H; i++) {
		// ��
		int dn = (i + W + W + H) * 5;
		points[dn + 0] = upperLeftX;
		points[dn + 1] = upperLeftY - ySpan + i * dy;
		points[dn + 2] = 0.0f;
		points[dn + 3] = 0.0f;
		points[dn + 4] = 1.0f - ((float)i)/((float)H);
		// ��
		dn = (i + W) * 5;
		points[dn + 0] = upperLeftX + xSpan;
		points[dn + 1] = upperLeftY - i * dy;
		points[dn + 2] = 0.0f;
		points[dn + 3] = 1.0f;
		points[dn + 4] = ((float)i) / ((float)H);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_STENCIL_TEST);//����ģ�����
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);//����ģ�建�������·�ʽ(��ͨ����λ��תģ��ֵ)

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);
	glStencilFunc(GL_ALWAYS, 1, 1);//��ʼģ��λΪ0������һ��ͨ�����ԣ�����ȫ���ᱻ��Ϊ1�����ظ���������(��EAB)���ڻ�������ģ��λ�ֹ�0
	glStencilMask(0x1);//����ģ�建����д��
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POLYGON, 0, totalPoints);

	glStencilFunc(GL_NOTEQUAL, 0, 1);//ģ��ֵ��Ϊ0��ͨ������EABƬ�λᱻ��������������ξ���ȷ������
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POLYGON, 0, totalPoints);

	glDisable(GL_STENCIL_TEST);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture);

	Draw();

	glUseProgram(0);

	glutSwapBuffers();
}

const float PI = glm::pi<float>();

void timerFunc() {
	if (startAngle <= 0) {
		startAngle = 2*PI;
	}
	else {
		startAngle -= 0.001f;
	}
	glutPostRedisplay();
}

int main() {

	glutInitWindowPosition(400, 200);
	glutInitWindowSize(720, 480);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL);
	glutCreateWindow("Hello, waving flag");

	glewInit();

	createShaderProgram();

	int width, height, nrChannels;
	unsigned char* data = stbi_load("guoqi/����1024.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutDisplayFunc(display);
	
	glutIdleFunc(timerFunc);

	glutMainLoop();

	return 0;
}