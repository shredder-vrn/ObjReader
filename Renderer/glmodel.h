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
    int vertexCount = 0;

    bool initialize(QOpenGLFunctions_3_3_Core * funcs);
    void render(QOpenGLFunctions_3_3_Core * funcs);
    void cleanup(QOpenGLFunctions_3_3_Core * funcs);
};

#endif // GLMODEL_H
