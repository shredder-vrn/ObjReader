#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QVector>
#include <QVector3D>

#include "Scene/scene.h"

class Viewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit Viewport(QWidget *parent = nullptr);
    ~Viewport() = default;

    void loadModel(const QString &filePath);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint lastMousePos;
    Scene scene;
};


#endif // VIEWPORT_H
