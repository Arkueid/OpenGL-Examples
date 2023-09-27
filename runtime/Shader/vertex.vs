#version 410 //版本号
//输入
layout (location = 0) in vec2 VertexPosition;
layout (location = 1) in vec3 VertexColor;

//输出
out vec3 Color;

//
uniform mat4 ProjectionMatrix;

void main()
{
    Color = VertexColor;
    gl_Position = ProjectionMatrix * vec4(VertexPosition,0,1.0);
}
