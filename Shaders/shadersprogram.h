#ifndef SHADERSPROGRAM_H
#define SHADERSPROGRAM_H

#include <QObject>
#include <QOpenGLShaderProgram>

class ShaderProgram : public QObject {
    Q_OBJECT

public:
    explicit ShaderProgram(QObject* parent = nullptr);
    virtual ~ShaderProgram();

    bool compileFromString(const QString& vertexSrc, const QString& fragmentSrc);
    bool compileFromFile(const QString& vertexPath, const QString& fragmentPath);

    QOpenGLShaderProgram* get() const;


private:
    QOpenGLShaderProgram *program = nullptr;
};

#endif // SHADERSPROGRAM_H
