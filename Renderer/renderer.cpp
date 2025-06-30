#include "renderer.h"

#include <QDebug>


bool OpenGLRenderer::initialize()
{
    qDebug() << "OpenGLRenderer :: initialize : запустили метод initialize";

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    shaderProgram = new ShaderProgram();

    if (!shaderProgram->compileFromString(
                R"(
                #version 330 core
                layout(location=0) in vec3 position;
                uniform mat4 projection;
                out vec3 positionOut;
                void main() {
                    gl_Position = vec4(position, 1.0) * projection;
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

bool OpenGLRenderer::initializeModel(const Model &model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    if (!openGLisInitialized) {
        qDebug() << "Досрочное завершение из-за openGLisInitialized";
        return false;}
    if (model.vertices.isEmpty()) {
        qDebug() << "Досрочное завершение из-за model.vertices.isEmpty()";
        return false;}

    if (openGLvao == 0)
        glGenVertexArrays(1, &openGLvao);
    if (openGLvbo == 0)
        glGenBuffers(1, &openGLvbo);

    glBindVertexArray(openGLvao);
    glBindBuffer(GL_ARRAY_BUFFER, openGLvbo);

    QVector<QVector3D> newArray = {};

    for (int i = 0; i < model.faceVertexIndices.size(); ++i) {
        int idx = model.faceVertexIndices[i];
        if (idx >= 0 && idx < model.vertices.size()) {
            newArray.append(model.vertices.value(idx));
        }
    }

    glBufferData(GL_ARRAY_BUFFER, newArray.size() * sizeof(QVector3D), newArray.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void OpenGLRenderer::setModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: setModel : запустили метод setModel";

    if (openGLvao != 0) {
        glDeleteVertexArrays(1, &openGLvao);
        openGLvao = 0;
    }

    if (openGLvbo != 0) {
        glDeleteBuffers(1, &openGLvbo);
        openGLvbo = 0;
    }

    openGLcurrentModel = model;
    initializeModel(openGLcurrentModel);

}

void OpenGLRenderer::setMVPmatrix(const QMatrix4x4& mvp)
{
    qDebug() << "OpenGLRenderer :: setViewProjectionMatrix : запустили метод setViewProjectionMatrix";

    openGLcurrentMvp = mvp;

    qDebug() << "openGLcurrentMvp: " << openGLcurrentMvp;

}

void OpenGLRenderer::render()
{
    qDebug() << "OpenGLRenderer :: render : запустили метод render";

    if (!openGLisInitialized) {
        qDebug() << "Рендеринг не выполнен из-за openGLisInitialized";
        return;}
    if (!shaderProgram) {
        qDebug() << "Рендеринг не выполнен из-за shaderProgram";
        return;}
    if (openGLcurrentModel.faceVertexIndices.isEmpty()) {
        qDebug() << "Рендеринг не выполнен из-за openGLcurrentModel.faceVertexIndices.isEmpty()";
        return;}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("projection", openGLcurrentMvp);

    static const QVector<QVector4D> colors = {
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f}
    };


    glBindVertexArray(openGLvao);

    for (int i = 0; i < openGLcurrentModel.polygonStarts.size(); ++i) {
        int start = openGLcurrentModel.polygonStarts[i];
        int count = (i < openGLcurrentModel.polygonStarts.size() - 1) ? openGLcurrentModel.polygonStarts[i+1] - start : openGLcurrentModel.faceVertexIndices.size() - start;

        glDrawArrays(GL_TRIANGLES, start, count);
        shaderProgram->get()->setUniformValue("faceColor", colors[i % colors.size()]);
    }

    glBindVertexArray(0);

    shaderProgram->get()->release();
}
