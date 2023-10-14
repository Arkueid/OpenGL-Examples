#include <iostream>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Sphere.hpp"
#include "Cube.hpp"
#include "Cylinder.hpp"
#include "Bezier.hpp"
#include "SinSurface.hpp"
#include "Camera.hpp"
#include <cgProgram.h>

#include <vector>

Camera* camera = NULL;
cgProgram* program = NULL;
std::vector<Drawable*> objects;

unsigned int texture;

int WindowWidth = 800;
int WindowHeight = 600;

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float aspect = (float)WindowWidth / (float)WindowHeight;
	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

	int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		objects[i]->SetProgram(program->GetHandle());
		objects[i]->SetViewMatrix(camera->GetViewMatrix());
		objects[i]->SetProjectionMatrix(projection);
		objects[i]->Render();
	}

	glutSwapBuffers();
}

// 控制摄像机位置
void keyboard(unsigned char key, int x, int y) {
	camera->ProcessKey(key, x, y);

	glutPostRedisplay();
}

// 控制摄像机角度
void specialKey(int key, int x, int y) {
	camera->ProcessSpecialKey(key, x, y);

	glutPostRedisplay();
}

void reshape(int width, int height) 
{
	WindowWidth = width;
	WindowHeight = height;
	glViewport(0, 0, width, height);
}


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Hello, project 3");

	glewInit();

	glEnable(GL_DEPTH_TEST);

	camera = new Camera();
	program = new cgProgram();

	program->CompileShader("Shader/general.vs");
	program->CompileShader("Shader/general.fs");

	//program->CompileShader("Shader/sphere.vs");
	//program->CompileShader("Shader/sphere.fs");

	//program->CompileShader("Shader/cube.vs");
	//program->CompileShader("Shader/cube.fs");

	//program->CompileShader("Shader/cylinder.vs");
	//program->CompileShader("Shader/cylinder.fs");

	program->Link();

	stbi_set_flip_vertically_on_load(true);
	// 纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/earth1.jpg", &width, &height, &nrChannels, 0);
	if (!data) {
		printf("failed to load texture.\n");
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Cube* cube = new Cube(1.0f);
	Cylinder* cylinder = new Cylinder(1.0f, 2.0f);
	std::vector<Point> points = { {0.0f,0.0f,0.0f}, {0.8f,0.0f,0.0f}, {0.8f, 2.0f, 0.0f}, {1.6f, 2.0f, 0.0f} }; // 贝塞尔曲线控制点，给定控制点的数量决定贝塞尔曲线的阶数
	Bezier* bezier = new Bezier(points);
	SinSurface* sinSurface = new SinSurface(-180.0f, 180.0f * 9.0f);

	cube->SetTexture(texture);
	cylinder->SetTexture(texture);
	bezier->SetTexture(texture);

	cube->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
	cylinder->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	bezier->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	sinSurface->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));

	//objects.push_back((Drawable*)bezier);
	//objects.push_back((Drawable*)cube);
	//objects.push_back((Drawable*)cylinder);
	objects.push_back((Drawable*)sinSurface);

	camera->Move(0.0f, 0.0f, 4.0f);

	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	delete camera;
	delete program;
	delete cube;
	delete cylinder;
	delete bezier;
	return 0;
}