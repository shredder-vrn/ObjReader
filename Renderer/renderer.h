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
    bool initialize();
    bool initializeModel(const Model& model);
    void render();  // Без параметров
    void resize(int w, int h);
    void setModel(const Model& model);
    void setViewProjectionMatrix(const QMatrix4x4& mvp);

private:
    ShaderProgram* shaderProgram = nullptr;
    GLuint vao = 0;
    GLuint vbo = 0;
    bool isInitialized = false;
    Model currentModel;          // Внутреннее хранение модели
    QMatrix4x4 currentMvp;      // Внутреннее хранение матрицы
};

#endif // RENDER_H
