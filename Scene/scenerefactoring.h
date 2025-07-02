#ifndef SCENEREFACTORING_H
#define SCENEREFACTORING_H

#include <QWidget>

#include "Model/model.h"
#include "Viewport/viewportrefactoring.h"
#include "ModelController/modelcontrollerrefactoring.h"

class SceneRefactoring : public QWidget
{
public:
    explicit SceneRefactoring(QWidget* parent = nullptr);

private:
    ModelControllerRefactoring m_model;
    ViewportRefactoring m_viewport;

};

#endif // SCENEREFACTORING_H
