#include "renderer.h"
#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>

bool OpenGLRenderer :: initialize()
{
    qDebug() << "OpenGLRenderer :: initialize : запустили метод initialize";

    QOpenGLExtraFunctions *glFuncs;

    glFuncs = QOpenGLContext::currentContext()->extraFunctions();

    glFuncs->glEnable(GL_DEPTH_TEST);
    glFuncs->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    const char *vertexShaderSource =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

    const char *fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";

    unsigned int vertexShader;

    vertexShader = glFuncs->glCreateShader(GL_VERTEX_SHADER);

    glFuncs->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glFuncs->glCompileShader(vertexShader);

    unsigned int fragmentShader;

    fragmentShader = glFuncs->glCreateShader(GL_FRAGMENT_SHADER);

    glFuncs->glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glFuncs->glCompileShader(fragmentShader);



    shaderProgram = glFuncs->glCreateProgram();

    glFuncs->glAttachShader(shaderProgram, vertexShader);
    glFuncs->glAttachShader(shaderProgram, fragmentShader);
    glFuncs->glLinkProgram(shaderProgram);

//    int  success;
//    char infoLog[512];
//    glFuncs->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if(!success) {
//        glFuncs->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//    }
//    qDebug()  << "SUCCESS: " << success;
//    qDebug() << infoLog;

    isInitialized = true;

    qDebug() << "OpenGLRenderer :: initialize : метод initialize отработал";

    return true;
}

bool OpenGLRenderer :: initializeModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    QVector<QVector3D> vertices = model.vertices;
    QOpenGLExtraFunctions *glFuncs;
    glFuncs = QOpenGLContext::currentContext()->extraFunctions();

    glFuncs->glGenVertexArrays(1, &vao);
    glFuncs->glBindVertexArray(vao);

    glFuncs->glGenBuffers(1, &vbo);
    glFuncs->glBindBuffer(GL_VERTEX_ARRAY, vbo);

    glFuncs->glUseProgram(shaderProgram);
    glFuncs->glBufferData(vbo, vertices.count() * sizeof(QVector3D), vertices.constData(), GL_STATIC_DRAW);


    glFuncs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    glFuncs->glEnableVertexAttribArray(0);

    glFuncs->glBindBuffer(GL_VERTEX_ARRAY, 0);
    glFuncs->glBindVertexArray(0);
    glFuncs->glUseProgram(0);

    qDebug() << "OpenGLRenderer :: initializeModel : метод initializeModel отработал";

    return true;
}

void OpenGLRenderer :: render()
{
    qDebug() << "OpenGLRenderer :: render : запустили метод render";

    if (vao == 0)
        return;
    QOpenGLExtraFunctions *glFuncs;
    glFuncs = QOpenGLContext::currentContext()->extraFunctions();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFuncs->glUseProgram(shaderProgram);
    glFuncs->glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFuncs->glBindVertexArray(0);
    glFuncs->glUseProgram(0);

    qDebug() << "OpenGLRenderer :: render : метод render отработал";
}

void OpenGLRenderer :: resize(int w, int h)
{
    qDebug() << "OpenGLRenderer :: resize : запустили метод resize";

    glViewport(0, 0, w, h);

    qDebug() << "OpenGLRenderer :: resize : метод resize отработал";
}
