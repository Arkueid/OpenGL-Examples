#include <iostream>
#include <vector>

#include <cgProgram.h>
#include "Cube.hpp"
#include "Camera.hpp"
#include "Drawable.hpp"

#include<GL/glew.h>
#include <GL/glut.h>

cgProgram generalProgram;
cgProgram lightProgram;
glm::vec3 lightPos(0.3f, 0.0f, 0.3f);
Camera camera;
std::vector<Drawable*> objects;

void display() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	const int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		if (i != size - 1) 
		{
			generalProgram.Use();
			generalProgram.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
			generalProgram.SetUniform("objectColor", glm::vec3(1.0f, 0.5f, 0.35f));
			generalProgram.SetUniform("lightPos", lightPos);
			generalProgram.SetUniform("viewPos", camera.Position);
			objects[i]->SetProjectionMatrix(glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f));
			objects[i]->SetViewMatrix(camera.GetViewMatrix());
			objects[i]->SetProgram(generalProgram.GetHandle());
			objects[i]->Render();
			generalProgram.Unuse();
		}
		else
		{
			lightProgram.Use();
			objects[i]->SetProjectionMatrix(glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f));
			objects[i]->SetViewMatrix(camera.GetViewMatrix());
			objects[i]->SetModelMatrix(glm::translate(glm::mat4(1.0f), lightPos));
			objects[i]->SetProgram(lightProgram.GetHandle());
			objects[i]->Render();
			lightProgram.Unuse();
		}
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) 
{
	camera.ProcessKey(key, x, y);
	glutPostRedisplay();
}

void specialKey(int key, int x, int y)
{
	camera.ProcessSpecialKey(key, x, y);
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

float angle = 0.0f;

void idle() 
{
	angle += 0.1;
	if (angle >= 360.0f) {
		angle = 0.0f;
	}
	lightPos = glm::vec3(glm::cos(glm::radians(angle)), 0.0f, glm::sin(glm::radians(angle)));
	glutPostRedisplay();
}

int main() 
{
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello, light");

	glewInit();
	glEnable(GL_DEPTH_TEST);

	// 创建普通着色器
	generalProgram.CompileShader("Shader/lighting.vs");
	generalProgram.CompileShader("Shader/lighting.fs");
	generalProgram.Link();

	// 创建光源着色器
	lightProgram.CompileShader("Shader/lightSource.vs");
	lightProgram.CompileShader("Shader/lightSource.fs");
	lightProgram.Link();

	// 创建正方体
	Cube cube(0.5f);
	cube.SetModelMatrix(glm::mat4(1.0f));
	objects.push_back((Drawable*)&cube);

	// 创建光源
	Cube light(0.2f);
	light.SetModelMatrix(glm::translate(glm::mat4(1.0f), lightPos));
	objects.push_back((Drawable*)&light);

	// 放置摄像机
	camera.Move(0.0f, 0.0f, 3.0f);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}