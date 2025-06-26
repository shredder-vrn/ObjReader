#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>

#include "Model/model.h"

class Viewport : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit Viewport(QWidget *parent = nullptr);
    void loadModel(const QString &filePath);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint lastMousePos;
    float angleX = 0.0f;
    float angleY = 0.0f;

    Model m_model;
    GLuint vbo = 0;
    GLuint vao = 0;
    QOpenGLShaderProgram *shaderProgram = nullptr;
};

#endif // VIEWPORT_H
