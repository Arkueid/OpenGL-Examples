#include "Plane.hpp"

Plane::Plane() {
    // 初始化成员变量
    ModelMatrix = glm::mat4(1.0f);
    ViewMatrix = glm::mat4(1.0f);
    ProjectionMatrix = glm::mat4(1.0f);
    TextureID = 0;
    Program = nullptr;

    // 生成VAO和VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // 创建顶点数据
    createData();
}

Plane::~Plane() {
    // 清理资源
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Plane::createData() {
    // 设置顶点数据，这里仅作示例
    float vertices[] = {
        // 顶点坐标
        -100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
         100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
         100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f, 
        -100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
        -100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
        100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f
    };

    vertexCount = 6;

    // 绑定VAO
    glBindVertexArray(vao);

    // 绑定VBO并上传数据
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // 解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 解绑VAO
    glBindVertexArray(0);
}

void Plane::Render() {
        // 设置模型矩阵、视图矩阵、投影矩阵
        Program->SetUniform("model", ModelMatrix);
        Program->SetUniform("view", ViewMatrix);
        Program->SetUniform("projection", ProjectionMatrix);
        Program->SetUniform("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));

        // 渲染
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
        glBindVertexArray(0);
}

void Plane::SetModelMatrix(glm::mat4 model) {
    ModelMatrix = model;
}

void Plane::SetViewMatrix(glm::mat4 view) {
    ViewMatrix = view;
}

void Plane::SetProjectionMatrix(glm::mat4 projection) {
    ProjectionMatrix = projection;
}

void Plane::SetTexture(unsigned int textureID) {
    TextureID = textureID;
}

void Plane::SetProgram(cgProgram* program) {
    Program = program;
}

cgProgram* Plane::GetProgram() {
    return Program;
}
