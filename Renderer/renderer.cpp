#include "renderer.h"

#include <QDebug>

OpenGLRenderer::OpenGLRenderer()
{
    qDebug() << "OpenGLRenderer :: OpenGLRenderer : запустили конструктор";

    qDebug() << "openGLvao" << openGLvao;
    qDebug() << "openGLvbo" << openGLvbo;
    qDebug() << "openGLisInitialized" << openGLisInitialized;
    qDebug() << "openGLcurrentMvp" << openGLcurrentMvp;
    qDebug() << "openGLcurrentModel.vertices" << openGLcurrentModel.vertices;
    qDebug() << "openGLcurrentModel.textureVertices" << openGLcurrentModel.textureVertices;
    qDebug() << "openGLcurrentModel.normals" << openGLcurrentModel.normals;
    qDebug() << "openGLcurrentModel.faceVertexIndices" << openGLcurrentModel.faceVertexIndices;
    qDebug() << "openGLcurrentModel.faceTextureVertexIndices" << openGLcurrentModel.faceTextureVertexIndices;
    qDebug() << "openGLcurrentModel.faceNormalIndices" << openGLcurrentModel.faceNormalIndices;


    qDebug() << "OpenGLRenderer :: OpenGLRenderer : конструктор отработал, все преобразования обработаны.";
}

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
                void main() {
                gl_Position = projection * vec4(position, 1.0);
                })",
                R"(
                #version 330 core
                out vec4 outColor;
                uniform vec4 faceColor;
                void main() {
                outColor = faceColor;
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

    qDebug() << "model.vertices" << model.vertices;
    qDebug() << "model.vertices" << model.textureVertices;
    qDebug() << "model.vertices" << model.normals;
    qDebug() << "model.faceVertexIndices" << model.faceVertexIndices;
    qDebug() << "model.faceTextureVertexIndices" << model.faceTextureVertexIndices;
    qDebug() << "model.faceNormalIndices" << model.faceNormalIndices;
    qDebug() << "model.polygonStarts" << model.polygonStarts;


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

    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(QVector3D), model.vertices.constData(), GL_STATIC_DRAW);
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

    qDebug() << "openGLvao" << openGLvao;
    qDebug() << "openGLvbo" << openGLvbo;
    qDebug() << "openGLisInitialized" << openGLisInitialized;
    qDebug() << "openGLcurrentMvp" << openGLcurrentMvp;
    qDebug() << "openGLcurrentModel.vertices" << openGLcurrentModel.vertices;
    qDebug() << "openGLcurrentModel.textureVertices" << openGLcurrentModel.textureVertices;
    qDebug() << "openGLcurrentModel.normals" << openGLcurrentModel.normals;
    qDebug() << "openGLcurrentModel.faceVertexIndices" << openGLcurrentModel.faceVertexIndices;
    qDebug() << "openGLcurrentModel.faceTextureVertexIndices" << openGLcurrentModel.faceTextureVertexIndices;
    qDebug() << "openGLcurrentModel.faceNormalIndices" << openGLcurrentModel.faceNormalIndices;
    qDebug() << "OpenGLRenderer :: render : запустили метод render";

    qDebug() << "Проверяем состояние openGLisInitialized: "<< openGLcurrentModel.faceVertexIndices.isEmpty();
    if (!openGLisInitialized) {
        qDebug() << "Рендеринг не выполнен из-за openGLisInitialized";
        return;
    }
    qDebug() << "Проверяем состояние shaderProgram: "<< shaderProgram;
    if (!shaderProgram) {
        qDebug() << "Рендеринг не выполнен из-за shaderProgram";
        return;
    }
    qDebug() << "Проверяем состояние !openGLcurrentModel.faceVertexIndices.isEmpty() "<< !openGLcurrentModel.faceVertexIndices.isEmpty();
    if (openGLcurrentModel.faceVertexIndices.isEmpty()) {
        qDebug() << "Рендеринг не выполнен из-за openGLcurrentModel.faceVertexIndices.isEmpty()";
        return;
    }

    qDebug() << "------------------------------------------------";
    qDebug() << "glClear() - GL_COLOR_BUFFER_BIT - очищает буфер цвета, GL_DEPTH_BUFFER_BIT - очищает буфер глубины";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->get()->bind();
    qDebug() << "get() - Получаем указатель на шейдерную программу" << shaderProgram;
    qDebug() << "bind() - Активирует шейдерную программу для всех операций отрисовки" << shaderProgram;

    shaderProgram->get()->setUniformValue("projection", openGLcurrentMvp);
    qDebug() << "get() - Получаем указатель на шейдерную программу" << shaderProgram;
    qDebug() << "setUniformValue() - Находим uniform-переменную 'projection' в шейдерной программе, копируем матрицу openGLcurrentMvp (тип QMatrix4x4) в uniform, под капотом вызываем glUniformMatrix4fv" << openGLcurrentMvp;

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
    qDebug() << "Устанавливаем цвета для закраски полигонов" << colors;


    glBindVertexArray(openGLvao);
    qDebug() << "glBindVertexArray() - Активируем текущий VAO" << openGLvao;

    glDrawArrays(GL_TRIANGLES, 0, 36);


    for (int i = 0; i < openGLcurrentModel.polygonStarts.size(); ++i) {
        int start = openGLcurrentModel.polygonStarts[i];
        int count = (i < openGLcurrentModel.polygonStarts.size() - 1) ? openGLcurrentModel.polygonStarts[i+1] - start : openGLcurrentModel.faceVertexIndices.size() - start;

        shaderProgram->get()->setUniformValue("faceColor", colors[i % colors.size()]);
        glDrawArrays(GL_TRIANGLES, start, count);
    }

    glBindVertexArray(0);
    qDebug() << "glBindVertexArray() - Деактивируем текущий VAO";

    shaderProgram->get()->release();



    qDebug() << "OpenGLRenderer :: render : метод render отработал";
    qDebug() << "------------------------------------------------";
}


void OpenGLRenderer::resize(int w, int h) {
    qDebug() << "OpenGLRenderer :: resize : запустили метод resize";

    glViewport(0, 0, w, h);

    qDebug() << "OpenGLRenderer :: resize : метод resize отработал";
}
