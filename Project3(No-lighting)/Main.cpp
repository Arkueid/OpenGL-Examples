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
#include "Ring.hpp"
#include "Camera.hpp"
#include <cgProgram.h>

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Camera camera;
std::vector<Drawable*> objects;

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
		if (i == 3 || i == 5) 
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		objects[i]->SetViewMatrix(camera.GetViewMatrix());
		objects[i]->SetProjectionMatrix(projection);
		objects[i]->Render();
		glDisable(GL_BLEND);
	}

	glutSwapBuffers();
}

// 控制摄像机位置
void keyboard(unsigned char key, int x, int y) {
	camera.ProcessKey(key, x, y);

	glutPostRedisplay();
}

// 控制摄像机角度
void specialKey(int key, int x, int y) {
	camera.ProcessSpecialKey(key, x, y);

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
	sphere->ModelMatrix *= translate;
	sphere->ModelMatrix *= glm::translate(glm::vec3(0.0f, -1.5f + 0.5f * glm::sin(glm::radians(angle)), 0.0f));
	sphere->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	sphere->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	sphere->ModelMatrix *= glm::translate(glm::vec3(0.24f * glm::cos(glm::radians(angle)), 0.0f, 0.24f * glm::sin(glm::radians(angle))));
	sphere->ModelMatrix *= scale;
	Ring* ring = (Ring*)objects[5];
	ring->ModelMatrix = glm::mat4(1.0f);
	ring->ModelMatrix *= translate;
	ring->ModelMatrix *= glm::translate(glm::vec3(0.0f, -1.5f + 0.5f * glm::sin(glm::radians(angle)), 0.0f));
	ring->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	ring->ModelMatrix *= glm::rotate(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	ring->ModelMatrix *= scale;

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
	textureProgram.CompileShader("Shader/texture.vs");
	textureProgram.CompileShader("Shader/texture.fs");
	textureProgram.Link();

	cgProgram program2;
	program2.CompileShader("Shader/general.vs");
	program2.CompileShader("Shader/general.fs");
	program2.Link();

	cgProgram program3;
	program3.CompileShader("Shader/cube-animation.vs");
	program3.CompileShader("Shader/cube-animation.fs");
	program3.Link();

	stbi_set_flip_vertically_on_load(true);
	unsigned int texture0, texture1, texture2;
	texture0 = load_texture("resources/earth1.jpg", GL_RGB, GL_RGB);
	texture1 = load_texture("resources/fish.bmp", GL_RGB, GL_RGB);
	texture2 = load_texture("resources/jui.jpg", GL_RGB, GL_RGB);

	Cube cube(1.0f);
	Cylinder cylinder(1.0f, 2.0f);
	std::vector<Point> points = { {0.0f,0.0f,0.0f}, {0.8f,0.0f,0.0f}, {0.8f, 2.0f, 0.0f}, {1.6f, 2.0f, 0.0f} }; // 贝塞尔曲线控制点，给定控制点的数量决定贝塞尔曲线的阶数
	Bezier bezier(points);
	SinSurface sinSurface(-180.0f, 180.0f * 9.0f);
	Sphere sphere(0.2f);
	Ring ring(0.3f, 0.8f);

	cube.SetProgram(program3.GetHandle());
	cylinder.SetProgram(textureProgram.GetHandle());
	bezier.SetProgram(textureProgram.GetHandle());
	sinSurface.SetProgram(program2.GetHandle());
	sphere.SetProgram(textureProgram.GetHandle());
	ring.SetProgram(program2.GetHandle());

	cube.SetTexture(texture1);
	cylinder.SetSideTexture(texture0);
	cylinder.SetTopTexture(texture2);
	bezier.SetTexture(texture0);
	sphere.SetTexture(texture0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	cube.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model *= glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cylinder.SetModelMatrix(model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.4f, -1.0f));
	model = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
	bezier.SetModelMatrix(model);

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

	objects.push_back((Drawable*)&cube);
	objects.push_back((Drawable*)&bezier);
	objects.push_back((Drawable*)&cylinder);
	objects.push_back((Drawable*)&sinSurface);
	objects.push_back((Drawable*)&sphere);
	objects.push_back((Drawable*)&ring);

	camera.Move(0.0f, 0.0f, 4.0f);

	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}