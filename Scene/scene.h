#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

#include "Camera/camera.h"
#include "Camera/сamerainterface.h"
#include "ModelController/modelcontroller.h"
#include "Renderer/renderer.h"

namespace Viewer  {

class Scene
{
public:
    Scene(){};
    ~Scene(){};

    void initialize();
    void loadModel(const QString &path);
    void render();
    void resize(const int width, const int height);

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
