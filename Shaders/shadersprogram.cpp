#include "shadersprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ShaderProgram::ShaderProgram(QObject* parent) : QObject(parent) {
    program = new QOpenGLShaderProgram(this);
}
ShaderProgram::~ShaderProgram() {
    delete program;
}

bool ShaderProgram::compileFromString(const QString& vertexSrc, const QString& fragmentSrc) {
    if (!program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc.toUtf8())) {
        qCritical() << "Vertex shader compilation error:" << program->log();
        return false;
    }

    if (!program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc.toUtf8())) {
        qCritical() << "Fragment shader compilation error:" << program->log();
        return false;
    }

    if (!program->link()) {
        qCritical() << "Shader program linking error:" << program->log();
        return false;
    }

    return true;
}

bool ShaderProgram::compileFromFile(const QString& vertexPath, const QString& fragmentPath) {
    QFile vFile(vertexPath);
    if (!vFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open vertex shader file:" << vertexPath;
        return false;
    }
    QString vSrc = QTextStream(&vFile).readAll();
    vFile.close();

    QFile fFile(fragmentPath);
    if (!fFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open fragment shader file:" << fragmentPath;
        return false;
    }
    QString fSrc = QTextStream(&fFile).readAll();
    fFile.close();

    return compileFromString(vSrc, fSrc);
}

QOpenGLShaderProgram* ShaderProgram::get() const {
    return program;
}
