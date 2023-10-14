// ProjectOne.cpp : 定义控制台应用程序的入口点。
//
#include "cgProgram.h" //一定要放在glew之前

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
	glClearColor(0.0, 0.5, 0.0, 0.0);//设置背景色
}

void LoadShader()
{
	
	//编译与链接Shader
	prog.CompileShader("Shader/general.vs");
	prog.CompileShader("Shader/general.fs");
	prog.Link();
}

void initTriangle(void)
{
	float * vertc = new float[3 * 2];
	float * color = new float[3 * 3];
	
	//定义三角形顶点坐标，法向量及纹理坐标
	int countv = 0;
	int countc = 0;

	//三角形属性数据（坐标和颜色）
	vertc[countv++] = 100.0f; vertc[countv++] = 100.0f;
	color[countc++] = 1.0f;   color[countc++] = 0.0f;   color[countc++] = 0.0f;
	
	vertc[countv++] = 500.0f; vertc[countv++] = 100.0f;
	color[countc++] = 0.0f;   color[countc++] = 1.0f;   color[countc++] = 0.0f;

	vertc[countv++] = 300.0f; vertc[countv++] = 400.0f;
	color[countc++] = 0.0f;   color[countc++] = 0.0f;    color[countc++] = 1.0f;

	glGenBuffers(2, vboHandle);//生成缓冲区，数据

	glGenVertexArrays(1, &vaoHandle);//生成顶点数组，与vertex shader绑定
	glBindVertexArray(vaoHandle);//可以多个顶点数组，使用时需要激活哪一个

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);//激活（绑定）当前缓冲区
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vertc, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader的顶点坐标输入属性（序号0，坐标）对应
	glEnableVertexAttribArray(0);  // 允许Vertex着色器中输入变量0读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);//缓冲区数据（颜色）和顶点绑定
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), color, GL_STATIC_DRAW);//内存数据复制到显存
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));//Vertex Shader的顶点颜色输入（序号1，颜色）属性对应
	glEnableVertexAttribArray(1);  //  // 允许Vertex着色器中输入变量1读取显存数据。

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	delete[] vertc;
	delete[] color;
}

void reshape(int width, int height)//窗口大小变化时执行
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	projection = glm::ortho(0.0f,(float)width,0.0f,(float)height);
}

void keyboard(unsigned char key, int x, int y)//响应键盘
{
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//启用需要的shader，可以动态切换不同的shader
    prog.Use();
	
	prog.SetUniform("ProjectionMatrix", projection);
	
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);//GL_LINES线段
	glBindVertexArray(0);

	//关闭所用的shader
	prog.Unuse();
	//glFlush();//单缓冲时必须要，说明绘图命令（函数）结束
	glutSwapBuffers();//交换缓冲（双缓冲时使用）
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//双缓冲，单缓冲为GLUT_SINGLE
	glutCreateWindow("Project One");

	glewInit();//OpenGL初始化,现代OpenGL需要
	init();

	LoadShader();//加载Shader
	initTriangle();//加载（生成）三角形数据

	glutDisplayFunc(display);//图形绘制
	glutReshapeFunc(reshape);//窗口大小变化
	glutKeyboardFunc(keyboard);//键盘交互

	glutMainLoop();

	glDeleteBuffers(2, vboHandle);//释放显存缓冲区
	glDeleteVertexArrays(1,&vaoHandle);//删除顶点数组对象

	return 0;
}