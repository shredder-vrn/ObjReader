//! REVIEW: VIEWPORT_H -> VIEWPORT_VIEWPORT_H
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QVector>
#include <QVector3D>
#include "Camera/сamerainterface.h"
#include "Camera/cameraper.h"
#include "Camera/cameraort.h"

#include "ModelController/modelcontroller.h"
#include "Renderer/renderer.h"


namespace Viewer{

class ViewportWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    ~ViewportWidget() override = default;

    void setModels(const QVector<Model*>& models, const QVector<QMatrix4x4>& transforms);

    void switchToPerspective();
    void switchToOrthographic();
    void fitToView();
    bool loadTextureForModel(const QString& texturePath, int modelIndex);


//*************************************
    void setModels(const QVector<ModelGL*>& models, const QVector<QMatrix4x4>& transforms);
//*************************************

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
    ModelController m_modelController;
    OpenGLRenderer m_renderer;
    std::unique_ptr<CameraInterface> m_camera;

    QVector<Model*> m_models;
    QVector<QMatrix4x4> m_modelTransforms;

    Model m_grid;
    Model m_worldAxes;
    Model m_cameraTarget;
    Model m_localAxes;



//*************************************
    QVector<ModelGL*> m_modelsGL;
    ModelGL m_gridGL;
    ModelGL m_worldAxesGL;
    ModelGL m_cameraTargetGL;
    ModelGL m_localAxesGL;
//*************************************
};
}


#endif // VIEWPORT_H
