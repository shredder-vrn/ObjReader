#include "renderer.h"

#include <QDebug>
#include <QImage>

namespace Viewer
{
bool OpenGLRenderer::initialize()
{
    qDebug() << "OpenGLRenderer :: initialize : запустили метод initialize";

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);



    shaderProgram = new ShaderProgram();

    if (!shaderProgram->compileFromString(
                R"(
                #version 330 core
                layout(location=0) in vec3 position;
                uniform mat4 openGLcurrentMvp;
                out vec3 positionOut;

                out vec2 texCoord;
                out vec3 normal;
                out vec3 fragPos;

                layout(location=1) in vec2 inTexCoord;
                out vec2 outTexCoord;

                void main() {
                    gl_Position = openGLcurrentMvp * vec4(position, 1.0);
                    outTexCoord = inTexCoord;
                    positionOut = position;
                })",
                R"(
                #version 330 core
                in vec2 outTexCoord;
                out vec4 outColor;
                in vec3 positionOut;
                uniform vec4 faceColor;
                uniform sampler2D modelTexture;
                uniform bool useTexture;

                void main() {
                    if (useTexture) {
                        outColor = texture(modelTexture, outTexCoord);
                    } else {
                        outColor = vec4(positionOut * 0.5 + 0.5, 1);
                    }
                })")) {

        qCritical("Failed to compile shaders");
        delete shaderProgram;
        shaderProgram = nullptr;
        return false;
    }
    openGLisInitialized = true;
    return true;
}

bool OpenGLRenderer::initializeModel(Model &model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    if (!openGLisInitialized) {
        qDebug() << "Досрочное завершение из-за openGLisInitialized";
        return false;
    }
    if (model.vertices.isEmpty()) {
        qDebug() << "Досрочное завершение из-за model.vertices.isEmpty()";
        return false;
    }

    if (model.vao != 0){
        glDeleteVertexArrays(1, &model.vao);
        model.vao = 0;
    }

    glGenVertexArrays(1, &model.vao);
    glBindVertexArray(model.vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    QVector<float> vertexData;
    for (int idx : model.faceVertexIndices) {
        QVector3D pos = model.vertices[idx];
        vertexData << pos.x() << pos.y() << pos.z();
    }

    QVector<float> texCoordData;
    for (int idx : model.faceTextureVertexIndices) {
        if (idx >= 0 && idx < model.textureVertices.size()) {
            QVector2D tex = model.textureVertices[idx];
            texCoordData << tex.x() << tex.y();
        }
    }

    QVector<float> fullData;
    for (int i = 0; i < vertexData.size(); i += 3) {
        fullData << vertexData[i] << vertexData[i+1] << vertexData[i+2];

        if (i / 3 < texCoordData.size() / 2) {
            fullData << texCoordData[(i / 3) * 2];
            fullData << texCoordData[(i / 3) * 2 + 1];
        } else {
            fullData << 0.0f << 0.0f;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, fullData.size() * sizeof(float), fullData.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    model.vertexCount = vertexData.size() / 3;
    glBindVertexArray(0);

    qDebug() << "[DEBUG] VAO создан для модели";
    qDebug() << "Количество вершин:" << model.vertexCount;
    qDebug() << "Количество текстурных координат:" << texCoordData.size();

    return true;
}

void OpenGLRenderer::render(const Model& model, const QMatrix4x4& mvp)
{
    if (!openGLisInitialized || !shaderProgram || !model.isValid()) {
        return;
    }

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("openGLcurrentMvp", mvp);
    shaderProgram->get()->setUniformValue("useTexture", model.hasTexture);

    if (model.vao == 0) {
        qDebug() << "VAO не создан — создаём сейчас";
        if (!initializeModel(const_cast<Model&>(model))) {
            shaderProgram->get()->release();
            return;
        }
    }

    glBindVertexArray(model.vao);

    if (model.hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.textureId);
        shaderProgram->get()->setUniformValue("modelTexture", 0);
    }

    for (int i = 0; i < model.polygonStarts.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = (i < model.polygonStarts.size() - 1)
                        ? model.polygonStarts[i + 1] - start
                        : model.faceVertexIndices.size() - start;

        glDrawArrays(GL_TRIANGLES, start, count);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    shaderProgram->get()->release();

    qDebug() << "[DEBUG] Модель отрисована";
    qDebug() << "hasTexture:" << model.hasTexture;
    qDebug() << "textureId:" << model.textureId;
}

bool OpenGLRenderer::loadTexture(Model &model, const QString &texturePath)
{
    if (!openGLisInitialized) return false;

    QImage textureImage(texturePath);
    if (textureImage.isNull()) {
        qWarning() << "Не удалось загрузить текстуру:" << texturePath;
        return false;
    }

    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888);
    if (textureImage.width() <= 0 || textureImage.height() <= 0) return false;

    glGenTextures(1, &model.textureId);
    glBindTexture(GL_TEXTURE_2D, model.textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    model.hasTexture = true;

    qDebug() << "[SUCCESS] Текстура загружена:" << texturePath;
    qDebug() << "Размер текстуры:" << textureImage.width() << "x" << textureImage.height();
    qDebug() << "textureId:" << model.textureId;

    return true;
}

void OpenGLRenderer::setModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: setModel : запустили метод setModel";
    openGLcurrentModel = model;
    initializeModel(openGLcurrentModel);
}

void OpenGLRenderer::setMVPmatrix(const QMatrix4x4& mvp)
{
    qDebug() << "OpenGLRenderer :: setViewProjectionMatrix : запустили метод setViewProjectionMatrix";
    openGLcurrentMvp = mvp;
    qDebug() << "openGLcurrentMvp: " << openGLcurrentMvp;
}
}
