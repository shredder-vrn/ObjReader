#include "shadersprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ShaderProgram::ShaderProgram(QObject *parent) : QObject(parent)
{
    qDebug() << "ShaderProgram :: ShaderProgram : запустили конструктор";
    myShaderProgram = new QOpenGLShaderProgram(this);
}

ShaderProgram::~ShaderProgram()
{
    qDebug() << "ShaderProgram :: ShaderProgram : запустили деструктор";
    delete myShaderProgram;
}

bool ShaderProgram::compileFromString(const QString& vertexSrc, const QString& fragmentSrc)
{
    qDebug() << "ShaderProgram :: compileFromString : запустили метод compileFromString";
    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc.toUtf8()))
        return false;
    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc.toUtf8()))
        return false;
    if (!myShaderProgram->link())
        return false;

    return true;    
}

bool ShaderProgram::compileFromFile(const QString& vertexPath, const QString& fragmentPath)
{
    qDebug() << "ShaderProgram :: compileFromFile : запустили метод compileFromFile";

    QFile vFile(vertexPath);

    if (!vFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open vertex shader file:" << vertexPath;
        return false;}

    QString vSrc = QTextStream(&vFile).readAll();
    vFile.close();

    QFile fFile(fragmentPath);

    if (!fFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open fragment shader file:" << fragmentPath;
        return false;}

    QString fSrc = QTextStream(&fFile).readAll();

    fFile.close();

    return compileFromString(vSrc, fSrc);
}

QOpenGLShaderProgram *ShaderProgram::get() const
{
    return myShaderProgram;
}
