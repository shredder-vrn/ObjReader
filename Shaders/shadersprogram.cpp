#include "shadersprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

ShaderProgram::ShaderProgram(QObject *parent) : QObject(parent) {
    qDebug() << "ShaderProgram :: ShaderProgram : запустили конструктор";

    myShaderProgram = new QOpenGLShaderProgram(this);

    qDebug() << "myShaderProgram" << myShaderProgram;

    qDebug() << "ShaderProgram :: ShaderProgram : конструктор отработал, все преобразования обработаны";

}

ShaderProgram::~ShaderProgram() {
    qDebug() << "ShaderProgram :: ShaderProgram : запустили деструктор";

    delete myShaderProgram;

    qDebug() << "ShaderProgram :: ShaderProgram : деструктор отработал";

}

bool ShaderProgram::compileFromString(const QString& vertexSrc, const QString& fragmentSrc) {

    qDebug() << "ShaderProgram :: compileFromString : запустили метод compileFromString";

    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc.toUtf8()))
        return false;
    qDebug() << "Добавили вершинный шейдер к шейдерной программе addShaderFromSourceCode():" << vertexSrc.data();


    if (!myShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc.toUtf8()))
        return false;
    qDebug() << "Добавили фрагментный шейдер к шейдерной программе addShaderFromSourceCode():" << fragmentSrc.data();

    if (!myShaderProgram->link())
        return false;
    qDebug() << "Линкуем myShaderProgram link():" << myShaderProgram->log();

    qDebug() << "Кол-во шейдеров в программе:" << myShaderProgram->shaders().count();

    qDebug() << "ShaderProgram :: compileFromString : метод compileFromString отработал, все преобразования обработаны";

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
    qDebug() << "myShaderProgram" << myShaderProgram;

    return myShaderProgram;

    qDebug() << "ShaderProgram :: get : метод get отработал";

}
