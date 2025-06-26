#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

#include "Camera/camera.h"
#include "ModelController/modelcontroller.h"
#include "Renderer/renderer.h"

class Scene {
public:
    Scene();
    ~Scene();

    void initialize();
    void render();
    void resize(int w, int h);
    void rotate(float dx, float dy);
    void loadModel();

    void setModelPath(const QString& path);

    bool hasModel() const;

private:
    Camera camera;
    ModelController modelController;
    OpenGLRenderer renderer;

    QString modelPath;
};

#endif // SCENE_H
