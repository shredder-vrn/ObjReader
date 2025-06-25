#include "viewport.h"

Viewport::Viewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void Viewport::initializeGL()
{
}

void Viewport::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height); // устанавливаем размер вьюпорта
}

void Viewport::paintGL()
{
    glClearColor(0.5, 0.5, 0.5, 1); // ставим цвет фона

    glClear(GL_COLOR_BUFFER_BIT); // очищаем экран
    glBegin(GL_TRIANGLES); // начинаем рисовать треугольники

    glColor3f(1, 0, 0); // задаем цвет вершины
    glVertex3f(0, 0, 0); // задаем координату вершины

    glColor3f(0, 1, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 1, 0);

    glEnd(); // заканчиваем рисование треугольников
}
