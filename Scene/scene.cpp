#include "scene.h"


<<<<<<< Updated upstream
Scene::Scene() : modelController(), renderer() {}

Scene::~Scene() = default;

void Scene::setModelPath(const QString& path) {
    modelPath = path;
}

void Scene::initialize() {
=======
Scene :: Scene() : modelController(), renderer()
{
    qDebug() << "Scene :: Scene : запустили конструктор";

    qDebug() << "Scene :: Scene : конструктор отработал";

}

void Scene :: setModelPath(const QString &path)
{
    qDebug() << "Scene :: setModelPath : запустили метод setModelPath";

    modelPath = path;

    qDebug() << "Scene :: setModelPath : метод setModelPath отработал";
}

void Scene :: initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";

>>>>>>> Stashed changes
    renderer.initialize();

    qDebug() << "Scene :: initialize : метод initialize отработал";

}

<<<<<<< Updated upstream
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
=======
void Scene :: loadModel()
{
    qDebug() << "Scene :: loadModel : запустили метод loadModel";

    renderer.initializeModel(Model());

    qDebug() << "Scene :: loadModel : метод loadModel отработал";
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";

    renderer.render();

    qDebug() << "Scene :: render : метод render отработал";
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";

>>>>>>> Stashed changes
    camera.setViewportSize(w, h);
    renderer.resize(w, h);

    qDebug() << "Scene :: resize : метод resize отработал";
}

<<<<<<< Updated upstream
void Scene::rotate(float dx, float dy) {
=======
void Scene :: rotate(float dx, float dy)
{
    qDebug() << "Scene :: rotate : запустили метод rotate";

>>>>>>> Stashed changes
    modelController.rotate(dx, dy);

    qDebug() << "Scene :: rotate : метод rotate отработал";
}

<<<<<<< Updated upstream
bool Scene::hasModel() const {
    return modelController.getModel().vertices.size() > 0;
=======
bool Scene :: hasModel() const
{
    qDebug() << "Scene :: hasModel : запустили метод hasModel";

    bool result = modelController.getModel().vertices.size() > 0;
    return result;

    qDebug() << "Scene :: hasModel : метод hasModel отработал";
>>>>>>> Stashed changes
}
