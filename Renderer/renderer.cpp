#include "renderer.h"
#include <QDebug>


OpenGLRenderer::OpenGLRenderer() = default;

OpenGLRenderer::~OpenGLRenderer() {
    cleanup();
}

bool OpenGLRenderer::initialize() {
    if (!initializeOpenGLFunctions()) {
        qCritical("Failed to initialize OpenGL functions");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    shaderProgram = new ShaderProgram();
    if (!shaderProgram->compileFromString(
        R"(
#version 330 core
layout(location=0) in vec3 position;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(position, 1.0);
}
)",
        R"(
#version 330 core
out vec4 outColor;
uniform vec4 faceColor;
void main() {
    outColor = faceColor;
}
)"
    )) {
        qCritical("Failed to compile shaders");
        delete shaderProgram;
        shaderProgram = nullptr;
        return false;
    }

    isInitialized = true;
    return true;
}

bool OpenGLRenderer::initializeModel(const Model& model) {
    if (!isInitialized || model.vertices.isEmpty()) {
        return false;
    }

    if (vao == 0) glGenVertexArrays(1, &vao);
    if (vbo == 0) glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 model.vertices.size() * sizeof(QVector3D),
                 model.vertices.constData(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


void OpenGLRenderer::render(const Model& model, const QMatrix4x4& mvp) {
    if (!isInitialized || !shaderProgram || model.faceVertexIndices.isEmpty()) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("projection", mvp);

    static const QVector<QVector4D> colors = {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };

    glBindVertexArray(vao);

    for (int i = 0; i < model.polygonStarts.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = (i < model.polygonStarts.size() - 1)
                  ? model.polygonStarts[i+1] - start
                  : model.faceVertexIndices.size() - start;

        shaderProgram->get()->setUniformValue("faceColor", colors[i % colors.size()]);
        glDrawArrays(GL_LINE_LOOP, start, count);
    }

    glBindVertexArray(0);
    shaderProgram->get()->release();
}


void OpenGLRenderer::resize(int w, int h) {
    glViewport(0, 0, w, h);
}

void OpenGLRenderer::cleanup() {
    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    delete shaderProgram;
    shaderProgram = nullptr;
    isInitialized = false;
}
