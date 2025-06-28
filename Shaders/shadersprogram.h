#ifndef SHADERSPROGRAM_H
#define SHADERSPROGRAM_H

#include <QObject>
#include <QOpenGLShaderProgram>

class ShaderProgram : public QObject {
    Q_OBJECT

public:
    ShaderProgram(QObject* parent = nullptr);
    ~ShaderProgram();

    bool compileFromString(const QString& vertexSrc, const QString& fragmentSrc);
    bool compileFromFile(const QString& vertexPath, const QString& fragmentPath);

    QOpenGLShaderProgram* get() const;


private:
    QOpenGLShaderProgram *myShaderProgram = nullptr;
};

#endif // SHADERSPROGRAM_H
