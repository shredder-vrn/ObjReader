#include "shadersprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Viewer {
ShaderProgram::ShaderProgram(QObject *parent) : QObject(parent)
{
    myShaderProgram = new QOpenGLShaderProgram(this);
}

ShaderProgram::~ShaderProgram()
{
    delete myShaderProgram;
}

bool ShaderProgram::compileFromString(const QString &vertexSrc, const QString &fragmentSrc)
{
    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc.toUtf8()))
        return false;
    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc.toUtf8()))
        return false;
    if (!myShaderProgram->link())
        return false;

    return true;    
}

bool ShaderProgram::compileFromFile(const QString &vertexPath, const QString &fragmentPath)
{
    QFile vFile(vertexPath);
    if (!vFile.exists()) {
        qDebug() << "[ERROR] Vertex shader file does not exist:" << vertexPath;
        return false;
    }

    if (!vFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[ERROR] Could not open vertex shader file:" << vertexPath;
        return false;
    }

    QTextStream vStream(&vFile);
    QString vertexSrc = vStream.readAll();
    vFile.close();

    QFile fFile(fragmentPath);
    if (!fFile.exists()) {
        qDebug() << "[ERROR] Fragment shader file does not exist:" << fragmentPath;
        return false;
    }

    if (!fFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[ERROR] Could not open fragment shader file:" << fragmentPath;
        return false;
    }

    QTextStream fStream(&fFile);
    QString fragmentSrc = fStream.readAll();
    fFile.close();

    qDebug() << "[INFO] Compiling shaders:";
    qDebug() << "Vertex shader path:" << vertexPath;
    qDebug() << "Fragment shader path:" << fragmentPath;

    return compileFromString(vertexSrc, fragmentSrc);
}

QOpenGLShaderProgram *ShaderProgram::get() const
{
    return myShaderProgram;
}
}
