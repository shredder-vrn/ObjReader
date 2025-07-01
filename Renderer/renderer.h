#ifndef RENDER_H
#define RENDER_H

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QMatrix4x4>

#include "Model/model.h"
#include "Shaders/shadersprogram.h"


class OpenGLRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    bool initialize();
    void render();

    bool initializeModel(const Model& model);

    void setModel(const Model& model);

    void setMVPmatrix(const QMatrix4x4& mvp);

private:
    ShaderProgram *shaderProgram = nullptr;

    GLuint openGLvao = 0;
    GLuint openGLvbo = 0;
    Model openGLcurrentModel;

    bool openGLisInitialized = false;

    QMatrix4x4 openGLcurrentMvp;
};

#endif // RENDER_H
