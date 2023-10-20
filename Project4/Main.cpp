#include "Drawable.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "cgProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>

cgProgram program;
cgProgram lightingProgram;
Camera camera;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 ambient(0.1f, 0.1f, 0.1f);
glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
glm::vec3 specular(1.0f, 1.0f, 1.0f);
std::vector<Drawable*> objects;

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(45.0f, 1.5f, 0.1f, 1000.0f);

	int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		if (i != size-1) 
		{
			lightingProgram.Use();
			lightingProgram.SetUniform("viewPos", camera.Position);
			lightingProgram.SetUniform("light.position", lightPos);
			lightingProgram.SetUniform("light.ambient", ambient);
			lightingProgram.SetUniform("light.diffuse", diffuse);
			lightingProgram.SetUniform("light.specular", specular);
			lightingProgram.SetUniform("material.diffuse", 0);
			lightingProgram.SetUniform("material.shineness", 32);
			lightingProgram.SetUniform("material.specular", glm::vec3(0.1f, 0.1f, 0.1f));
		}
		else 
		{
			program.Use();
		}
		objects[i]->SetViewMatrix(camera.GetViewMatrix());
		objects[i]->SetProjectionMatrix(projection);
		objects[i]->Render();
		lightingProgram.Unuse();
		program.Unuse();
	}

	glutSwapBuffers();
}

float angle = 0.0f;
float orbitAngle = 0.0f;
float angle2 = 0.0f;

void idle() 
{
	Drawable* earth = objects[0];
	glm::mat4 model(1.0f);
	float R = 10.0f;
	float orbitRadius = glm::radians(orbitAngle);
	model = glm::translate(model, glm::vec3(R * glm::cos(orbitRadius), 0.0f, R * glm::sin(orbitRadius)));
	model = glm::rotate(model, glm::radians(-23.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	earth->SetModelMatrix(model);

	Drawable* sun = objects[1];
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle2), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	sun->SetModelMatrix(model);

	angle += 0.01f;
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}

	orbitAngle += 0.005f;
	if (orbitAngle >= 360.0f)
	{
		orbitAngle = 0.0f;
	}

	angle2 += 0.0005f;
	if (angle2 >= 360.0f)
	{
		angle2 = 0.0f;
	}
	glutPostRedisplay();
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

unsigned int load_texture(const char* fileName, int internalFormat, int format)
{
	unsigned int texture0;
	int width, height, nChannel;
	// R -> 灰度图
	unsigned char* data = stbi_load(fileName, &width, &height, &nChannel, 0);
	if (!data)
	{
		std::string log;
		log += "failed to load texture: ";
		log += fileName;
		throw std::runtime_error(log);
	}
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	// 多级别纹理映射
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture0;
}

void mouse(int button, int state, int x, int y)
{
	camera.ProcessMouse(button, state, x, y);
	glutPostRedisplay();
}

void motion(int x, int y)
{
	camera.ProcessMotion(x, y);
	glutPostRedisplay();
}

int main()
{
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Hello, lighting");

	glewInit();
	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);

	// 创建着色器
	program.CompileShader("Shader/sphere.vs");
	program.CompileShader("Shader/sphere.fs");
	program.Link();

	lightingProgram.CompileShader("Shader/lightingTexture.vs");
	lightingProgram.CompileShader("Shader/lightingTexture.fs");
	lightingProgram.Link();

	unsigned int texture0 = load_texture("resources/earth2.jpg", GL_RGB, GL_RGB);
	unsigned int texture1 = load_texture("resources/sun.jpg", GL_RGB, GL_RGB);

	// 创建物体
	// 地球
	Sphere earth(0.5f);
	earth.SetProgram(lightingProgram.GetHandle());
	earth.SetTexture(texture0);
	objects.push_back((Drawable*)&earth);
	// 太阳
	Sphere sun(3.0f);
	sun.SetProgram(program.GetHandle());
	sun.SetTexture(texture1);
	objects.push_back((Drawable*)&sun);

	// 放置摄像机
	camera.Move(0.0f, 0.0f, 20.0f);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();
	return 0;
}