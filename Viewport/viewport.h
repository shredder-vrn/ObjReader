//! REVIEW: VIEWPORT_H -> VIEWPORT_VIEWPORT_H
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QVector>
#include <QVector3D>

#include "Scene/scene.h"

namespace Viewer{




//! REVIEW: namespace
//! REVIEW: Viewport -> ViewportWidget
class Viewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit Viewport(QWidget *parent = nullptr);
    ~Viewport() override = default;
    //! REVIEW: override
    //!
    //! class Widget
    //! {
    //! public:
    //!     virtual ~Widget() = default;
    //!     virtual void f()
    //!     {
    //!         print("I'm Parent");
    //!     }
    //! };
    //! class Viewport : public Widget
    //! {
    //! public:
    //!     ~Viewport() override
    //!     {
    //!         delete glObjects;
    //!     }
    //!
    //!     void f() override
    //!     {
    //!         print("I'm Child");
    //!     }
    //! };
    //!
    //! Widget *p = new Viewport();
    //! p->f();
    //! delete p;
    //!

    void loadModel(const QString &filePath);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


private:
    QPoint lastMousePos;
    Scene scene;
    //! REVIEW:
    //! Scene *scene = nullptr;
    //! if (scene == nullptr)
    //!
};
}


#endif // VIEWPORT_H
