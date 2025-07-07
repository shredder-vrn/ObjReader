#ifndef RENDER_H
#define RENDER_H

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QMatrix4x4>

#include "Model/modelgl.h"
#include "Shaders/shadersprogram.h"

namespace Viewer{
class OpenGLRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    bool initialize();
    void setMVPmatrix(const QMatrix4x4 &mvp);



//*************************************
    void render(const ModelGL &modelGL, const QMatrix4x4 &mvp);
    bool initializeModel(ModelGL &modelGL);
    bool loadTexture(ModelGL &modelGL, const QString &texturePath);
    void setModelGL(const ModelGL &modelGL);
//*************************************


private:
    ShaderProgram *shaderProgram = nullptr;
    bool m_lightingCheck = false;

    GLuint openGLvao = 0;
    GLuint openGLvbo = 0;
    bool openGLisInitialized = false;
    QMatrix4x4 openGLcurrentMvp;



//*************************************
    ModelGL openGLcurrentModelGL;
//*************************************
};
}
#endif // RENDER_H
