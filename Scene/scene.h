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
    Scene();
    ~Scene() = default;

    void initialize();
    void loadModel();
    void render();

    void resize(int w, int h);

    void rotate(float dx, float dy);

    void setModelPath(const QString& path);

    bool hasModel() const;

private:
    ModelController modelController;
    OpenGLRenderer renderer;
    Camera camera;

    QString modelPath;
};

#endif // SCENE_H
