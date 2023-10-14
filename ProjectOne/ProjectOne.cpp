// ProjectOne.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "cgProgram.h" //һ��Ҫ����glew֮ǰ

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using glm::vec4;

cgProgram prog;

unsigned int vboHandle[2];
unsigned int vaoHandle;

mat4 projection;

void init(void)
{
	glClearColor(0.0, 0.5, 0.0, 0.0);//���ñ���ɫ
}

void LoadShader()
{
	
	//����������Shader
	prog.CompileShader("Shader/general.vs");
	prog.CompileShader("Shader/general.fs");
	prog.Link();
}

void initTriangle(void)
{
	float * vertc = new float[3 * 2];
	float * color = new float[3 * 3];
	
	//���������ζ������꣬����������������
	int countv = 0;
	int countc = 0;

	//�������������ݣ��������ɫ��
	vertc[countv++] = 100.0f; vertc[countv++] = 100.0f;
	color[countc++] = 1.0f;   color[countc++] = 0.0f;   color[countc++] = 0.0f;
	
	vertc[countv++] = 500.0f; vertc[countv++] = 100.0f;
	color[countc++] = 0.0f;   color[countc++] = 1.0f;   color[countc++] = 0.0f;

	vertc[countv++] = 300.0f; vertc[countv++] = 400.0f;
	color[countc++] = 0.0f;   color[countc++] = 0.0f;    color[countc++] = 1.0f;

	glGenBuffers(2, vboHandle);//���ɻ�����������

	glGenVertexArrays(1, &vaoHandle);//���ɶ������飬��vertex shader��
	glBindVertexArray(vaoHandle);//���Զ���������飬ʹ��ʱ��Ҫ������һ��

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//����󶨣���ǰ������
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vertc, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader�Ķ��������������ԣ����0�����꣩��Ӧ
	glEnableVertexAttribArray(0);  // ����Vertex��ɫ�����������0��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//���������ݣ���ɫ���Ͷ����
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), color, GL_STATIC_DRAW);//�ڴ����ݸ��Ƶ��Դ�
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader�Ķ�����ɫ���루���1����ɫ�����Զ�Ӧ
	glEnableVertexAttribArray(1);  //  // ����Vertex��ɫ�����������1��ȡ�Դ����ݡ�

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	delete[] vertc;
	delete[] color;
}

void reshape(int width, int height)//���ڴ�С�仯ʱִ��
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f,(float)width,0.0f,(float)height);
}

void keyboard(unsigned char key, int x, int y)//��Ӧ����
{
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//������Ҫ��shader�����Զ�̬�л���ͬ��shader
    prog.Use();
	
	prog.SetUniform("ProjectionMatrix", projection);
	
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);//GL_LINES�߶�
	glBindVertexArray(0);

	//�ر����õ�shader
	prog.Unuse();
	//glFlush();//������ʱ����Ҫ��˵����ͼ�������������
	glutSwapBuffers();//�������壨˫����ʱʹ�ã�
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//˫���壬������ΪGLUT_SINGLE
	glutCreateWindow("Project One");

	glewInit();//OpenGL��ʼ��,�ִ�OpenGL��Ҫ
	init();

	LoadShader();//����Shader
	initTriangle();//���أ����ɣ�����������

	glutDisplayFunc(display);//ͼ�λ���
	glutReshapeFunc(reshape);//���ڴ�С�仯
	glutKeyboardFunc(keyboard);//���̽���

	glutMainLoop();

	glDeleteBuffers(2, vboHandle);//�ͷ��Դ滺����
	glDeleteVertexArrays(1,&vaoHandle);//ɾ�������������

	return 0;
}