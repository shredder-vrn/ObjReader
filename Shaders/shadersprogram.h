#ifndef VIEWER_SHADERSPROGRAM_H
#define VIEWER_SHADERSPROGRAM_H

#include <QObject>
#include <QOpenGLShaderProgram>

namespace Viewer {

class ShaderProgram : public QObject {
    Q_OBJECT

public:
    ShaderProgram(QObject *parent = nullptr);
    ~ShaderProgram();

    bool compileFromString(const QString &vertexSrc, const QString &fragmentSrc);
    bool compileFromFile(const QString &vertexPath, const QString &fragmentPath);

    QOpenGLShaderProgram *get() const;


private:
    //! REVIEW: правило 5 или 0
    QOpenGLShaderProgram *myShaderProgram = nullptr;
};
}

#endif // VIEWER_SHADERSPROGRAM_H
