#include "renderer.h"

#include <QDebug>


bool OpenGLRenderer::initialize()
{
    qDebug() << "OpenGLRenderer :: initialize : запустили метод initialize";

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    qDebug() << "Инициализируем глубину отрисовки glEnable(GL_DEPTH_TEST)";

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    qDebug() << "Инициализируем цвет фона     glClearColor(0.2f, 0.2f, 0.2f, 1.0f);";

    shaderProgram = new ShaderProgram();
    qDebug() << "Инициализируем shaderProgram" << shaderProgram;

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
    qDebug() << "Компилируем shaderProgram" << shaderProgram;

    openGLisInitialized = true;
    qDebug() << "openGLisInitialized" << openGLisInitialized;

    qDebug() << "OpenGLRenderer :: initialize : метод initialize отработал, все преобразования обработаны";
    return true;
}

bool OpenGLRenderer::initializeModel(const Model &model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    qDebug() << "Проверяем openGLisInitialized" << openGLisInitialized;
    if (!openGLisInitialized) {
        qDebug() << "Досрочное завершение из-за openGLisInitialized";
        return false;
    }
    qDebug() << "Проверяем !model.vertices.isEmpty()" << !model.vertices.isEmpty();
    if (model.vertices.isEmpty()) {
        qDebug() << "Досрочное завершение из-за model.vertices.isEmpty()";
        return false;
    }


    qDebug() << "openGLvao" << openGLvao;
    if (openGLvao == 0)
        glGenVertexArrays(1, &openGLvao);
    qDebug() << "glGenVertexArrays() - генерируем уникальный идентификатор для openGLvao" << openGLvao;
    qDebug() << "openGLvbo" << openGLvbo;
    if (openGLvbo == 0)
        glGenBuffers(1, &openGLvbo);
    qDebug() << "glGenBuffers() - генерируем уникальный идентификатор для openGLvbo" << openGLvbo;


    glBindVertexArray(openGLvao);
    qDebug() << "glBindVertexArray() - активируем openGLvbo с указанным идентификатором" << openGLvao;
    glBindBuffer(GL_ARRAY_BUFFER, openGLvbo);
    qDebug() << "glBindBuffer() - активируем openGLvbo с указанным идентификатором" << openGLvbo;

    QVector<QVector3D> newArray = {};

    for (int vertexIndex = 0; vertexIndex < model.faceVertexIndices.size(); ++vertexIndex) {
        newArray.append(model.vertices.at(model.faceVertexIndices[vertexIndex]));
    }

    glBufferData(GL_ARRAY_BUFFER, newArray.size() * sizeof(QVector3D), newArray.constData(), GL_STATIC_DRAW);
    qDebug() << "glBufferData() - загружаем данные вершин в привязанный VBO: "
             << "GL_ARRAY_BUFFER: " << GL_ARRAY_BUFFER
             << "model.vertices.size() * sizeof(QVector3D): " << model.vertices.size() * sizeof(QVector3D)
             << "model.vertices.constData(): " << model.vertices.constData() << "GL_STATIC_DRAW: " << GL_STATIC_DRAW;

    qDebug() << "model.vertices.size()" << model.vertices.size();
    qDebug() << "sizeof(QVector3D)" <<  sizeof(QVector3D);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    qDebug() << "glVertexAttribPointer() - Указываем формат атрибута: " << 0 << ", " << 3 << ", GL_FLOAT: " << GL_FLOAT << ", sizeof(QVector3D): " << sizeof(QVector3D) << ", nullptr";


    glEnableVertexAttribArray(0);
    qDebug() << "glEnableVertexAttribArray() - Включаем использование вершинного атрибута с индексом 0 в текущем шейдере";


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    qDebug() << "glBindBuffer() - Деактивируем текущий VBO от цели GL_ARRAY_BUFFER";


    glBindVertexArray(0);
    qDebug() << "glBindVertexArray() - Деактивируем текущий VAO";


    qDebug() << "OpenGLRenderer :: initializeModel : метод initializeModel отработал, все преобразования обработаны.";

    return true;
}

void OpenGLRenderer::setModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: setModel : запустили метод setModel";

    openGLcurrentModel = model;
    initializeModel(openGLcurrentModel);

    qDebug() << "OpenGLRenderer :: setModel : метод setModel отработал";
}

void OpenGLRenderer::setViewProjectionMatrix(const QMatrix4x4& mvp)
{
    qDebug() << "OpenGLRenderer :: setViewProjectionMatrix : запустили метод setViewProjectionMatrix";
    openGLcurrentMvp = mvp;
    qDebug() << "OpenGLRenderer :: setViewProjectionMatrix : метод setViewProjectionMatrix отработал";
}

void OpenGLRenderer::render()
{
    qDebug() << "OpenGLRenderer :: render : запустили метод render";

    if (!openGLisInitialized) {
        qDebug() << "Рендеринг не выполнен из-за openGLisInitialized";
        return;
    }
    if (!shaderProgram) {
        qDebug() << "Рендеринг не выполнен из-за shaderProgram";
        return;
    }
    if (openGLcurrentModel.faceVertexIndices.isEmpty()) {
        qDebug() << "Рендеринг не выполнен из-за openGLcurrentModel.faceVertexIndices.isEmpty()";
        return;
    }

    qDebug() << "------------------------------------------------";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->get()->bind();

    shaderProgram->get()->setUniformValue("projection", openGLcurrentMvp.transposed());

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
    qDebug() << "glBindVertexArray() - Деактивируем текущий VAO";

    shaderProgram->get()->release();

    qDebug() << "OpenGLRenderer :: render : метод render отработал";
}
