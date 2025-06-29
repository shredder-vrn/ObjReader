#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QVector>
#include <QVector3D>

#include "Scene/scene.h" // <-- новый класс


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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Scene scene;

    QPoint viewportLastMousePos;
};


#endif // VIEWPORT_H
