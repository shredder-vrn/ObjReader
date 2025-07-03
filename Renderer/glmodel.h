#ifndef GLMODEL_H
#define GLMODEL_H

#include "Model/model.h"
#include <QOpenGLFunctions_3_3_Core>

class GLModel
{
public:
    GLModel();
    Model model;
    GLuint vao = 0;
    GLuint vbo = 0;
    int indexCount = 0;
};

#endif // GLMODEL_H
