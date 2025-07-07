#include "renderer.h"

#include <QDebug>
#include <QImage>

namespace Viewer {
bool OpenGLRenderer::initialize()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    shaderProgram = new ShaderProgram();

    QString vertexShaderPath = "../Shaders/vertexshader.glsl";
    QString fragmentShaderPath = "../Shaders/fragmentshader.glsl";



    if (!shaderProgram->compileFromFile(vertexShaderPath, fragmentShaderPath)) {
        qCritical("Failed to compile shaders from file");
        delete shaderProgram;
        shaderProgram = nullptr;
        return false;
    }

    openGLisInitialized = true;
    return true;
}

bool OpenGLRenderer::initializeModel(Model &model)
{
    if (!openGLisInitialized || model.vertices().isEmpty()) {
        return false;
    }

    GLuint vao = model.vao();

    if (model.vao() != 0){
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    glGenVertexArrays(1, &vao);
    model.setVao(vao);
    glBindVertexArray(model.vao());

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    QVector<float> fullData;

    const auto &faceIndices = model.faceVertexIndices();
    const auto &normals = model.normals();
    const auto &textureVerts = model.textureVertices();
    const auto &textureIndices = model.faceTextureVertexIndices();

    fullData.reserve(faceIndices.size() * 8);

    for (int i = 0; i < faceIndices.size(); ++i) {
        int idx = faceIndices[i];

        QVector3D pos = model.vertices().value(idx, QVector3D());
        fullData << pos.x() << pos.y() << pos.z();

        QVector3D normal = normals.value(idx, QVector3D());
        fullData << normal.x() << normal.y() << normal.z();

        if (i < textureIndices.size()) {
            int texIdx = textureIndices[i];
            if (texIdx >= 0 && texIdx < textureVerts.size()) {
                QVector2D tex = textureVerts[texIdx];
                fullData << tex.x() << tex.y();
            } else {
                fullData << 0.0f << 0.0f;
            }
        } else {
            fullData << 0.0f << 0.0f;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, fullData.size() * sizeof(float), fullData.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    model.setVertexCount(faceIndices.size());

    glBindVertexArray(0);

    return true;
}

void OpenGLRenderer::render(const Model &model, const QMatrix4x4 &mvp)
{
    if (!openGLisInitialized || !shaderProgram || !model.isValid()) {
        return;
    }

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("openGLcurrentMvp", mvp);
    shaderProgram->get()->setUniformValue("useNormal", model.useNormals());
    shaderProgram->get()->setUniformValue("useTexture", model.hasTexture());

    if (model.vao() == 0) {
        if (!initializeModel(const_cast<Model&>(model))) {
            shaderProgram->get()->release();
            return;
        }
    }

    glBindVertexArray(model.vao());

    if (model.hasTexture()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.textureId());
        shaderProgram->get()->setUniformValue("modelTexture", 0);
    }

    for (int i = 0; i < model.polygonStarts().size(); ++i) {
        int start = model.polygonStarts()[i];
        int count = (i < model.polygonStarts().size() - 1)
                        ? model.polygonStarts()[i + 1] - start
                        : model.faceVertexIndices().size() - start;

        glDrawArrays(GL_TRIANGLES, start, count);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    shaderProgram->get()->release();
}

bool OpenGLRenderer::loadTexture(Model &model, const QString &texturePath)
{
    if (!openGLisInitialized)
        return false;

    QImage textureImage(texturePath);
    if (textureImage.isNull())
        return false;

    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888).mirrored();

    if (textureImage.width() <= 0 || textureImage.height() <= 0)
        return false;

    GLuint textureId = model.textureId();
    if (textureId != 0)
        glDeleteTextures(1, &textureId);

    glGenTextures(1, &textureId);
    model.setTextureId(textureId);
    model.setHasTexture(true);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 textureImage.width(), textureImage.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    model.setHasTexture(true);

    return true;
}

void OpenGLRenderer::setModel(const Model &model)
{
    openGLcurrentModel = model;
    initializeModel(openGLcurrentModel);
}

void OpenGLRenderer::setMVPmatrix(const QMatrix4x4 &mvp)
{
    openGLcurrentMvp = mvp;
}



//*************************************
void OpenGLRenderer::render(const ModelGL &modelGL, const QMatrix4x4 &mvp)
{

}

bool OpenGLRenderer::loadTexture(ModelGL &modelGL, const QString &texturePath)
{

}

void OpenGLRenderer::setModelGL(const ModelGL &modelGL)
{

}
//*************************************
}
