#include "scene.h"


Scene::Scene() : modelController(), renderer() {}

Scene::~Scene() = default;

void Scene::setModelPath(const QString& path) {
    modelPath = path;
}

void Scene::initialize() {
    renderer.initialize();
}

void Scene::loadModel() {
    if (modelPath.isEmpty()) {
        qWarning() << "No model path set.";
        return;
    }
    if (!modelController.loadModel(modelPath)) {
        qWarning() << "Failed to load model from:" << modelPath;
        return;
    }
    if (!renderer.initializeModel(modelController.getModel())) {
        qWarning() << "Failed to initialize model in renderer";
    }
}

void Scene::render() {
    if (!hasModel()) return;

    QMatrix4x4 mvp = camera.getProjectionMatrix() *
                     camera.getViewMatrix() *
                     modelController.getModelMatrix();

    renderer.render(modelController.getModel(), mvp);
}

void Scene::resize(int w, int h) {
    camera.setViewportSize(w, h);
    renderer.resize(w, h);
}

void Scene::rotate(float dx, float dy) {
    modelController.rotate(dx, dy);
}

bool Scene::hasModel() const {
    return modelController.getModel().vertices.size() > 0;
}
