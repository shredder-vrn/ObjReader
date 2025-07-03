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
//    glDisable(GL_CULL_FACE);



    shaderProgram = new ShaderProgram();

    if (!shaderProgram->compileFromString(
                R"(
                #version 330 core
                layout(location=0) in vec3 position;
                uniform mat4 openGLcurrentMvp;
                out vec3 positionOut;
                void main() {
                    gl_Position = openGLcurrentMvp * vec4(position, 1.0);
                    positionOut = position;
                })",
                R"(
                #version 330 core
                out vec4 outColor;
                in vec3 positionOut;
                uniform vec4 faceColor;
                void main() {
                    outColor = vec4(positionOut * 0.5 + 0.5, 1);
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


    QVector<QVector3D> positionData = model.vertices;
    QVector<QVector2D> texCoordData = model.textureVertices;

    QVector<QVector3D> newArray;

    for (int i = 0; i < model.faceVertexIndices.size(); ++i) {
        int idx = model.faceVertexIndices[i];
        if (idx >= 0 && idx < model.vertices.size()) {
            newArray.append(model.vertices.value(idx));
        }
    }

    glBufferData(GL_ARRAY_BUFFER, newArray.size() * sizeof(QVector3D), newArray.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    glEnableVertexAttribArray(0);

    model.vertexCount = newArray.size();

    glBindVertexArray(0);

    return true;
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

void OpenGLRenderer::render(const Model& model, const QMatrix4x4& mvp)
{
    if (!openGLisInitialized || !shaderProgram || !model.isValid()) {
        return;
    }


    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("openGLcurrentMvp", mvp);

    if (model.vao == 0) {
        qDebug() << "VAO не создан — создаём сейчас";
        if (!initializeModel(const_cast<Model&>(model))) {
            qDebug() << "Не удалось создать VAO";
            shaderProgram->get()->release();
            return;
        }
    }

    glBindVertexArray(model.vao);

    for (int i = 0; i < model.polygonStarts.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = (i < model.polygonStarts.size() - 1)
                        ? model.polygonStarts[i + 1] - start
                        : model.faceVertexIndices.size() - start;
        glDrawArrays(GL_TRIANGLES, start, count);
    }

    glBindVertexArray(0);
    shaderProgram->get()->release();
}
}
