#include "shadersprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ShaderProgram::ShaderProgram(QObject *parent) : QObject(parent) {
    qDebug() << "ShaderProgram :: ShaderProgram : запустили конструктор";

    program = new QOpenGLShaderProgram(this);
    qDebug() << "ShaderProgram :: ShaderProgram : конструктор отработал";

}

ShaderProgram::~ShaderProgram() {
    qDebug() << "ShaderProgram :: ShaderProgram : запустили деструктор";

    delete program;
    qDebug() << "ShaderProgram :: ShaderProgram : деструктор отработал";

}

bool ShaderProgram::compileFromString(const QString& vertexSrc, const QString& fragmentSrc) {

    qDebug() << "ShaderProgram :: compileFromString : запустили метод compileFromString";

    if (!program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc.toUtf8())) {
        return false;
    }

    if (!program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc.toUtf8())) {
        return false;
    }

    if (!program->link()) {
        return false;
    }

    qDebug() << "ShaderProgram :: compileFromString : метод compileFromString отработал";

    return true;
}

bool ShaderProgram::compileFromFile(const QString& vertexPath, const QString& fragmentPath) {
    qDebug() << "ShaderProgram :: compileFromFile : запустили метод compileFromFile";

    QFile vFile(vertexPath);
    if (!vFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open vertex shader file:" << vertexPath;
        return false;
    }
    QString vSrc = QTextStream(&vFile).readAll();
    vFile.close();

    QFile fFile(fragmentPath);
    if (!fFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open fragment shader file:" << fragmentPath;
        return false;
    }
    QString fSrc = QTextStream(&fFile).readAll();
    fFile.close();

    qDebug() << "ShaderProgram :: compileFromFile : метод compileFromFile отработал";
    return compileFromString(vSrc, fSrc);
}

QOpenGLShaderProgram *ShaderProgram::get() const {
    qDebug() << "ShaderProgram :: get : запустили метод get";

    return program;

    qDebug() << "ShaderProgram :: get : метод get отработал";

}
