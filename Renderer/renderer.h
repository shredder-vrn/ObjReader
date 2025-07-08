#ifndef RENDER_H
#define RENDER_H

#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QDebug>
#include <QImage>
#include <QMatrix4x4>

#include "ObjectsGL/modelgl.h"
#include "Shaders/shadersprogram.h"

namespace Viewer{

class OpenGLRenderer : protected QOpenGLFunctions_3_3_Core {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    bool initialize();
    bool initializeModel(ModelGL &modelGL);
    bool loadTexture(ModelGL &modelGL, const QString &filePath);
    bool loadTextureFromImage(ModelGL &modelGL, const QImage &image);
    bool loadTextureFromData(ModelGL &modelGL, const unsigned char *data, int width, int height, GLenum format = GL_RGBA);

    void render(const ModelGL &modelGL, const QMatrix4x4 &mvp);
    void setModelGL(const ModelGL &modelGL);
    void setMVPmatrix(const QMatrix4x4 &mvp);


private:
    ShaderProgram *shaderProgram = nullptr;
    bool m_lightingCheck = false;

    GLuint openGLvao = 0;
    GLuint openGLvbo = 0;
    bool openGLisInitialized = false;
    QMatrix4x4 openGLcurrentMvp;
    ModelGL openGLcurrentModelGL;

};
}
#endif // RENDER_H
