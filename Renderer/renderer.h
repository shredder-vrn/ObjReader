#ifndef RENDER_H
#define RENDER_H

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QMatrix4x4>

#include "Model/model.h"
#include "Shaders/shadersprogram.h"

namespace Viewer{
class OpenGLRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    bool initialize();
    void render(const Model &model, const QMatrix4x4 &mvp);

    bool initializeModel(Model &model);
    bool loadTexture(Model &model, const QString &texturePath);

    void setModel(const Model &model);
    void setMVPmatrix(const QMatrix4x4 &mvp);

private:
    ShaderProgram *shaderProgram = nullptr;
    bool m_lightingCheck = false;


    GLuint openGLvao = 0;
    GLuint openGLvbo = 0;
    Model openGLcurrentModel;
    bool openGLisInitialized = false;
    QMatrix4x4 openGLcurrentMvp;
};
}
#endif // RENDER_H
