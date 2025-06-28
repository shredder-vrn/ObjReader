#include "renderer.h"

#include <QDebug>

OpenGLRenderer::OpenGLRenderer()
{
    qDebug() << "OpenGLRenderer :: OpenGLRenderer : запустили конструктор";

    qDebug() << "openGLvao" << openGLvao;
    qDebug() << "openGLvbo" << openGLvbo;
    qDebug() << "openGLisInitialized" << openGLisInitialized;

    qDebug() << "OpenGLRenderer :: OpenGLRenderer : конструктор отработал";
}

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
        void main() {
        gl_Position = projection * vec4(position, 1.0);
        }
        )",
        R"(
        #version 330 core
        out vec4 outColor;
        uniform vec4 faceColor;
        void main() {
            outColor = faceColor;
        }
        )"
        )) {
        qCritical("Failed to compile shaders");
        delete shaderProgram;
        shaderProgram = nullptr;
        return false;
        }

    openGLisInitialized = true;

    qDebug() << "OpenGLRenderer :: initialize : метод initialize отработал";
    return true;
}

bool OpenGLRenderer::initializeModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    if (!openGLisInitialized || model.vertices.isEmpty()) {
        return false;
    }

    if (openGLvao == 0) glGenVertexArrays(1, &openGLvao);
    if (openGLvbo == 0) glGenBuffers(1, &openGLvbo);

    glBindVertexArray(openGLvao);
    glBindBuffer(GL_ARRAY_BUFFER, openGLvbo);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(QVector3D), model.vertices.constData(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    qDebug() << "OpenGLRenderer :: initializeModel : метод initializeModel отработал";

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

    if (!openGLisInitialized || !shaderProgram || openGLcurrentModel.faceVertexIndices.isEmpty()) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->get()->bind();
    shaderProgram->get()->setUniformValue("projection", openGLcurrentMvp);

    static const QVector<QVector4D> colors = {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };

    glBindVertexArray(openGLvao);

    for (int i = 0; i < openGLcurrentModel.polygonStarts.size(); ++i) {
        int start = openGLcurrentModel.polygonStarts[i];
        int count = (i < openGLcurrentModel.polygonStarts.size() - 1)
                  ? openGLcurrentModel.polygonStarts[i+1] - start
                  : openGLcurrentModel.faceVertexIndices.size() - start;

        shaderProgram->get()->setUniformValue("faceColor", colors[i % colors.size()]);
        glDrawArrays(GL_LINE_LOOP, start, count);
    }

    glBindVertexArray(0);
    shaderProgram->get()->release();

    qDebug() << "OpenGLRenderer :: render : метод render отработал";
}


void OpenGLRenderer::resize(int w, int h) {
    qDebug() << "OpenGLRenderer :: resize : запустили метод resize";

    glViewport(0, 0, w, h);

    qDebug() << "OpenGLRenderer :: resize : метод resize отработал";
}


