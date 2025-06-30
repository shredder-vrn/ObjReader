#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

#include "Camera/camera.h"
#include "ModelController/modelcontroller.h"
#include "Renderer/renderer.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    void setModelPath(const QString& path);
    void initialize();
    void loadModel(Model &model);
    void render();
    void resize(int w, int h);
    bool hasModel() const;

private:
    int sceneWidth = 600;
    int sceneHeight = 800;

    ModelController modelController;
    OpenGLRenderer renderer;
    Camera camera;

    QString modelPath = "";
};

#endif // SCENE_H
