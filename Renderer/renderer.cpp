#include "renderer.h"

#include "Viewport/logger.h"

namespace Viewer {

OpenGLRenderer::OpenGLRenderer()
{

}

OpenGLRenderer::~OpenGLRenderer()
{

}

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

void OpenGLRenderer::setMVPmatrix(const QMatrix4x4 &mvp)
{
    openGLcurrentMvp = mvp;
}

void OpenGLRenderer::render(const ModelGL &modelGL, const QMatrix4x4 &mvp)
{
    if (!openGLisInitialized || !shaderProgram || !modelGL.isValid()) {
        return;
    }

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("openGLcurrentMvp", mvp);
    shaderProgram->get()->setUniformValue("useNormal", modelGL.useNormals());
    shaderProgram->get()->setUniformValue("useTexture", modelGL.useTexture());

    if (modelGL.vao() == 0) {
        if (!initializeModel(const_cast<ModelGL&>(modelGL))) {
            shaderProgram->get()->release();
            return;
        }
    }

    if (modelGL.wireframeMode()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glBindVertexArray(modelGL.vao());

    if (modelGL.useTexture()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, modelGL.textureId());
        shaderProgram->get()->setUniformValue("modelTexture", 0);
    }

    const ModelData *data = modelGL.getModelData();
    if (!data) {
        glBindVertexArray(0);
        shaderProgram->get()->release();
        return;
    }

    const auto &polygonStarts = data->polygonStarts();
    for (int i = 0; i < polygonStarts.size(); ++i) {
        int start = polygonStarts[i];
        int count = (i < polygonStarts.size() - 1)
                        ? polygonStarts[i + 1] - start
                        : data->faceVertexIndices().size() - start;

        glDrawArrays(GL_TRIANGLES, start, count);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    shaderProgram->get()->release();
}

bool OpenGLRenderer::initializeModel(ModelGL &modelGL)
{
    if (!openGLisInitialized || !modelGL.getModelData() || modelGL.getModelData()->vertices().isEmpty()) {
        return false;
    }

    const ModelData *data = modelGL.getModelData();

    GLuint vao = modelGL.vao();

    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    glGenVertexArrays(1, &vao);
    modelGL.setVao(vao);
    glBindVertexArray(modelGL.vao());

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    QVector<float> fullData;
    fullData.reserve(data->faceVertexIndices().size() * 8);

    const auto &faceIndices = data->faceVertexIndices();
    const auto &normals = data->normals();
    const auto &textureVerts = data->textureVertices();
    const auto &textureIndices = data->faceTextureVertexIndices();

    for (int i = 0; i < faceIndices.size(); ++i) {
        int idx = faceIndices[i];

        QVector3D pos = data->vertices().value(idx, QVector3D());
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

    modelGL.setVertexCount(faceIndices.size());

    glBindVertexArray(0);

    return true;
}

bool OpenGLRenderer::loadTexture(ModelGL &modelGL, const QString &texturePath)
{
    if (!openGLisInitialized)
        return false;

    QImage textureImage(texturePath);
    if (textureImage.isNull())
        return false;

    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888).mirrored();

    if (textureImage.width() <= 0 || textureImage.height() <= 0)
        return false;

    GLuint textureId = modelGL.textureId();
    if (textureId != 0)
        glDeleteTextures(1, &textureId);

    glGenTextures(1, &textureId);
    modelGL.setTextureId(textureId);
    modelGL.setHasTexture(true);

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

    return true;
}

bool OpenGLRenderer::loadTextureFromImage(ModelGL &modelGL, const QImage &image)
{
    if (image.isNull()) {
        return false;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    QImage flippedImage = image.convertToFormat(QImage::Format_RGBA8888).mirrored();
    const uchar* data = flippedImage.constBits();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, flippedImage.width(), flippedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    modelGL.setHasTexture(true);
    modelGL.setTextureId(textureId);

    return true;
}

bool OpenGLRenderer::loadTextureFromData(ModelGL &modelGL, const unsigned char *data, int width, int height, GLenum format)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    modelGL.setHasTexture(true);
    modelGL.setTextureId(textureId);

    return true;
}

void OpenGLRenderer::setModelGL(const ModelGL &modelGL)
{
    openGLcurrentModelGL = modelGL;
    initializeModel(openGLcurrentModelGL);
}

}
