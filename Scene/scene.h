#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

#include "Camera/camera.h"
#include "ModelController/modelcontroller.h"
#include "Renderer/renderer.h"

//! REVIEW: namespace
//! Viewport.pri
//! Scene.pri
//! Navigation.pri
//! namespace Viewer

namespace Viewer  {

class Scene
{
public:
    Scene() = default;
    //! REVIEW: virtual
    ~Scene() = default;

    void initialize();
    void loadModel(const QString &path);
    void render();
    //! REVIEW: const int width
    //! w -> width
    //! h -> height
    void resize(int w, int h);

    Camera &getCamera() { return m_camera; }

private:
    int m_sceneWidth = 600;
    int m_sceneHeight = 800;

    ModelController m_modelController;
    OpenGLRenderer m_renderer;
    Camera m_camera;

    QString m_modelPath = "";
};

}

#endif // SCENE_H
