#ifndef VIEWPORT_VIEWPORT_H
#define VIEWPORT_VIEWPORT_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
#include <limits>

#include "Camera/сamerainterface.h"
#include "Camera/cameraper.h"
#include "Camera/cameraort.h"
#include "Renderer/renderer.h"
#include "ObjReader/objreader.h"



namespace Viewer{

class ViewportWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    ~ViewportWidget() override = default;

    void switchToPerspective();
    void switchToOrthographic();
    void fitToView();
    //! REVIEW: все ещё текстура только через диск. пу-пу-пу
    bool loadTextureForModel(const QString& texturePath, int modelIndex);
    void setModels(const QVector<ModelGL*>& models, const QVector<QMatrix4x4>& transforms);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void createGrid(float size = 10.0f, float step = 1.0f);
    void createWorldAxes(float size = 1.0f);
    void createLocalAxes(float size = 1.0f);

    QPoint m_lastMousePos;
    int m_viewportWidth = 600;
    int m_viewportHeight = 800;
    OpenGLRenderer m_renderer;
    std::unique_ptr<CameraInterface> m_camera;

    QVector<QMatrix4x4> m_modelTransforms;

    QVector<ModelGL*> m_modelsGL;
    ModelGL m_gridGL;
    ModelGL m_worldAxesGL;
    ModelGL m_cameraTargetGL;
    ModelGL m_localAxesGL;
};
}


#endif // VIEWPORT_H
