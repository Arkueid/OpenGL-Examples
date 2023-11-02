#include "Plane.hpp"

Plane::Plane() {
    // ��ʼ����Ա����
    ModelMatrix = glm::mat4(1.0f);
    ViewMatrix = glm::mat4(1.0f);
    ProjectionMatrix = glm::mat4(1.0f);
    TextureID = 0;
    Program = nullptr;

    // ����VAO��VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // ������������
    createData();
}

Plane::~Plane() {
    // ������Դ
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Plane::createData() {
    // ���ö������ݣ��������ʾ��
    float vertices[] = {
        // ��������
        -100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
         100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
         100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f, 
        -100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
        -100.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f,
        100.0f, 0.0f, 100.0f, 0.0f, 1.0f, 0.0f
    };

    vertexCount = 6;

    // ��VAO
    glBindVertexArray(vao);

    // ��VBO���ϴ�����
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ���VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ���VAO
    glBindVertexArray(0);
}

void Plane::Render() {
        // ����ģ�;�����ͼ����ͶӰ����
        Program->SetUniform("model", ModelMatrix);
        Program->SetUniform("view", ViewMatrix);
        Program->SetUniform("projection", ProjectionMatrix);
        Program->SetUniform("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));

        // ��Ⱦ
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
