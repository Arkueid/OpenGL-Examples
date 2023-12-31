#include <iostream>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Sphere.hpp"
#include "Cube.hpp"
#include "Cylinder.hpp"
#include "Curve.hpp"
#include "SinSurface.hpp"
#include "Ring.hpp"
#include "Plane.hpp"
#include "Camera.hpp"
#include <cgProgram.h>

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Camera camera;
std::vector<IDrawable*> objects;
std::vector<IDrawable*> lights;

glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
glm::vec3 ambient(0.1f, 0.1f, 0.1f);
glm::vec3 diffuse(1.0f, 0.8f, 0.5f);
glm::vec3 specular(1.0f, 0.8f, 0.5f);

int WindowWidth = 800;
int WindowHeight = 600;
glm::mat4 projection = glm::perspective(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 1000.0f);

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float aspect = (float)WindowWidth / (float)WindowHeight;

	int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		cgProgram* program = objects[i]->GetProgram();
		program->Use();
		program->SetUniform("viewPos", camera.Position);
		program->SetUniform("light.position", lightPos);
		program->SetUniform("light.ambient", ambient);
		program->SetUniform("light.diffuse", diffuse);
		program->SetUniform("light.specular", specular);
		if (i == 0 || i == 1 || i == 2 || i == 4)
			program->SetUniform("material.diffuse", 0);
		else program->SetUniform("material.diffuse", vec3(1.0f, 0.0f, 0.0f));
		program->SetUniform("material.shineness", 64.0f);
		program->SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		program->SetUniform("viewPos", camera.Position);

		objects[i]->SetViewMatrix(camera.GetViewMatrix());
		objects[i]->SetProjectionMatrix(projection);
		objects[i]->Render();

		program->Unuse();
	}
	size = lights.size();
	for (int i = 0; i < size; i++)
	{
		lights[i]->SetViewMatrix(camera.GetViewMatrix());
		lights[i]->SetProjectionMatrix(projection);
		lights[i]->Render();
	}
	glutSwapBuffers();
}

float moveStep = 0.1f;
bool autoOrbit = false;

void keyboard(unsigned char key, int x, int y) {
	camera.ProcessKey(key, x, y);
	switch (key)
	{
	case 'n':
	case 'N':
		lightPos.y += moveStep;
		break;
	case 'm':
	case 'M':
		lightPos.y -= moveStep;
		break;
	case 'r':
	case 'R':
		autoOrbit = !autoOrbit;
		lightPos.x = lightPos.z = 0.0f;
		lightPos.y = 1.0f;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		lightPos.z -= moveStep;
		break;
	case GLUT_KEY_DOWN:
		lightPos.z += moveStep;
		break;
	case GLUT_KEY_LEFT:
		lightPos.x -= moveStep;
		break;
	case GLUT_KEY_RIGHT:
		lightPos.x += moveStep;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void motion(int x, int y)
{
	camera.ProcessMotion(x, y);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	camera.ProcessMouse(button, state, x, y);
	glutPostRedisplay();
}

void reshape(int width, int height) 
{
	WindowWidth = width;
	WindowHeight = height;
	glViewport(0, 0, width, height);
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	camera.LastMousePos.x = (float)width / 2;
	camera.LastMousePos.y = (float)height / 2;
}

// 纹理截取的左下角坐标
// 截取的矩形长宽均为0.2f
int texSOffseti = 0;
int texTOffseti = 8;
int frameCount = 0;
float angle = 0.0f;

void idle()
{
	frameCount += 1;
	if (frameCount < 100000) return;
	else frameCount = 0;

	if (texSOffseti == 8)
	{
		if (texTOffseti == 0)// 归位
		{
			texTOffseti = 8;
			texSOffseti = 0;  
		}
		else  // 往下一行截取
		{
			texTOffseti -= 2;
			texSOffseti = 0;
		}
	}
	else
	{
		texSOffseti += 2;
	}

	glm::mat4 scale = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 translate = glm::translate(glm::vec3(-1.0f, 2.8f, 0.0f));
	Cube* cube = (Cube*) objects[0];
	cube->UpdateTexure((float)texSOffseti/10.0f, (float)texTOffseti/10.0f);
	Sphere* sphere = (Sphere*)objects[4];
	sphere->ModelMatrix = glm::mat4(1.0f);
	sphere->ModelMatrix *= glm::translate(lightPos);
	sphere->ModelMatrix *= scale;
	Ring* ring = (Ring*)objects[5];
	ring->ModelMatrix = glm::mat4(1.0f);
	ring->ModelMatrix *= translate;
	ring->ModelMatrix *= glm::translate(glm::vec3(0.0f, -1.2f + 0.5f * glm::sin(glm::radians(angle)), 0.0f));
	ring->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	ring->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	ring->ModelMatrix *= scale;

	if (autoOrbit) {
		lightPos.x = 2.5f * glm::cos(glm::radians(angle));
		lightPos.z = 2.5f * glm::sin(glm::radians(angle));
		lightPos.y = 3.0f * glm::sin(glm::radians(angle));
	}

	angle += 1.0f;
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}
	glutPostRedisplay();
}

unsigned int load_texture(const char* fileName, int internalFormat, int format)
{
	unsigned int texture0;
	int width, height, nChannel;
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

int main() {
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Hello, project 3");

	glewInit();

	glEnable(GL_DEPTH_TEST);

	cgProgram textureProgram;
	textureProgram.CompileShader("Shader/lightingTexture.vs");
	textureProgram.CompileShader("Shader/lightingTexture.fs");
	textureProgram.Link();

	cgProgram program2;
	program2.CompileShader("Shader/colorLighting.vs");
	program2.CompileShader("Shader/colorLighting.fs");
	program2.Link();

	cgProgram program3;
	program3.CompileShader("Shader/lighting/cube-animation.vs");
	program3.CompileShader("Shader/lighting/cube-animation.fs");
	program3.Link();

	cgProgram program4;
	program4.CompileShader("Shader/lightSource.vs");
	program4.CompileShader("Shader/lightSource.fs");
	program4.Link();

	stbi_set_flip_vertically_on_load(true);
	unsigned int texture0, texture1, texture2, texture3;
	texture0 = load_texture("resources/earth1.jpg", GL_RGB, GL_RGB);
	texture1 = load_texture("resources/fish.bmp", GL_RGB, GL_RGB);
	texture2 = load_texture("resources/jui.jpg", GL_RGB, GL_RGB);
	texture3 = load_texture("resources/sun.jpg", GL_RGB, GL_RGB);

	Cube cube(1.0f);
	Cylinder cylinder(1.0f, 2.0f);
	Curve curve1;
	SinSurface sinSurface(180.0f, 180.0f * 9.0f);
	Sphere sphere(0.2f);
	Ring ring(0.3f, 0.8f);
	Plane plane;

	cube.SetProgram(&program3);
	cylinder.SetProgram(&textureProgram);
	curve1.SetProgram(&textureProgram);
	sinSurface.SetProgram(&program2);
	sphere.SetProgram(&program4);
	ring.SetProgram(&program2);
	plane.SetProgram(&program2);

	cube.SetTexture(texture1);
	cylinder.SetSideTexture(texture0);
	cylinder.SetTopTexture(texture2);
	curve1.SetTexture(texture0);
	sphere.SetTexture(texture3);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	cube.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.3f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model *= glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cylinder.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.4f, -1.0f));
	model = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	curve1.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	sinSurface.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
	sphere.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
	ring.SetModelMatrix(model);

	objects.push_back((IDrawable*)&cube);
	objects.push_back((IDrawable*)&curve1);
	objects.push_back((IDrawable*)&cylinder);
	objects.push_back((IDrawable*)&sinSurface);
	objects.push_back((IDrawable*)&sphere);
	objects.push_back((IDrawable*)&ring);
	objects.push_back((IDrawable*)&plane);

	camera.Move(0.0f, 1.0f, 4.0f);

	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}