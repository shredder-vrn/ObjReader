#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include "ObjReader/objreader.h"
#include "viewport.h"
#include "Model/model.h"

Viewport::Viewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

void Viewport::loadModel(const QString &filePath)
{
    Model model;
    if (!parseObj(filePath, model)) {
        qDebug() << "[Ошибка] Не удалось загрузить модель из файла:" << filePath;
        return;
    }

    m_model = model;

    qDebug() << "[OK] Модель успешно загружена!";
    qDebug() << "Вершин (v):" << m_model.vertices.size();
    qDebug() << "Полигонов (f):" << m_model.polygonStarts.size();
}

void Viewport::initializeGL()
{
    if (!initializeOpenGLFunctions()) {
        qDebug() << "[Ошибка] Не удалось инициализировать OpenGL Functions!";
        return;
    }

    qDebug() << "[OK] OpenGL Functions успешно инициализированы!";

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        uniform mat4 projection;
        out vec4 color;
        void main()
        {
            gl_Position = projection * vec4(position, 1.0);
        }
    )");

    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        #version 330 core
        out vec4 outColor;
        uniform vec4 faceColor;
        void main()
        {
            outColor = faceColor;
        }
    )");

    shaderProgram->link();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (!m_model.vertices.isEmpty()) {
        glBufferData(GL_ARRAY_BUFFER,
                     m_model.vertices.size() * sizeof(QVector3D),
                     m_model.vertices.constData(),
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Viewport::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Viewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (vao == 0 || m_model.faceVertexIndices.isEmpty()) {
        qDebug() << "[paintGL] VAO == 0 или индексы пусты -> выход";
        return;
    }

    QMatrix4x4 modelMatrix;
    modelMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
    modelMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);

    QMatrix4x4 viewMatrix;
    viewMatrix.translate(0.0f, 0.0f, -3.0f);

    QMatrix4x4 projection;
    projection.perspective(45.0f, width() / (float)height(), 0.1f, 100.0f);

    QMatrix4x4 mvp = projection * viewMatrix * modelMatrix;

    shaderProgram->bind();
    shaderProgram->setUniformValue("projection", mvp);

    glBindVertexArray(vao);

    QVector<QVector4D> colors = {
        QVector4D(1.0f, 0.0f, 0.0f, 1.0f),
        QVector4D(0.0f, 1.0f, 0.0f, 1.0f),
        QVector4D(0.0f, 0.0f, 1.0f, 1.0f),
        QVector4D(1.0f, 1.0f, 0.0f, 1.0f),
        QVector4D(1.0f, 0.0f, 1.0f, 1.0f),
        QVector4D(0.0f, 1.0f, 1.0f, 1.0f)
    };

    for (int i = 0; i < m_model.polygonStarts.size(); ++i) {
        int start = m_model.polygonStarts[i];
        int next = (i < m_model.polygonStarts.size() - 1)
                       ? m_model.polygonStarts[i + 1]
                       : m_model.faceVertexIndices.size();
        int count = next - start;

        QVector4D color = colors[i % colors.size()];
        shaderProgram->setUniformValue("faceColor", color);

        glDrawArrays(GL_TRIANGLE_FAN, start, count);
    }

    glBindVertexArray(0);
    shaderProgram->release();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "[OpenGL Error]" << err;
    }
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
}

void Viewport::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - lastMousePos;

    angleY += delta.x() * 0.2f;
    angleX += delta.y() * 0.2f;

    lastMousePos = event->pos();
    update(); // Перерисовка
}
