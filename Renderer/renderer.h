#ifndef RENDER_H
#define RENDER_H

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QMatrix4x4>

#include "Model/model.h"
#include "Shaders/shadersprogram.h"

class ShaderProgram;


class OpenGLRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    bool initialize();
    bool initializeModel(const Model &model);
    void render();
    void resize(int w, int h);

private:
    void setupShaders();
    void setupBuffers(const Model& model);

    GLuint vao = 0;
    GLuint vbo = 0;
    ShaderProgram *shaderProgram = nullptr;
    bool isInitialized = false;
};

#endif // RENDER_H
