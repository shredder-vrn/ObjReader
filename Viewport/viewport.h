#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>

class Viewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    Viewport(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
};


#endif // VIEWPORT_H
