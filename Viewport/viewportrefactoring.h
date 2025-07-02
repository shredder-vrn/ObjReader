#ifndef VIEWPORTREFACTORING_H
#define VIEWPORTREFACTORING_H
#include "Camera/camerarefactoring.h"
#include "Renderer/rendererrefactoring.h"

class ViewportRefactoring
{
public:
    ViewportRefactoring();

private:
    CameraRefactoring m_camera;
    RendererRefactoring m_renderer;

};

#endif // VIEWPORTREFACTORING_H
