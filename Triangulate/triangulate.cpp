#include "triangulate.h"
#include <cmath>

#include <QDebug>

bool isConvex(const QVector<QVector3D>& allVertices, int prevIndex, int currIndex, int nextIndex) {
    QVector3D p = allVertices[prevIndex];
    QVector3D c = allVertices[currIndex];
    QVector3D n = allVertices[nextIndex];

    QVector3D edge1 = c - p;
    QVector3D edge2 = n - c;

    QVector3D cross = QVector3D::crossProduct(edge1, edge2);

    return cross.z() > 0; // Предполагаем плоскость XY
}


bool isInTriangle(const QVector3D& p,
                  const QVector3D& a,
                  const QVector3D& b,
                  const QVector3D& c)
{
    float area = 0.5f * (-b.y() * c.x() + a.y() * (-b.x() + c.x()) + a.x() * (b.y() - c.y()) + b.x() * c.y() - c.x() * b.y());

    if (area == 0)
        return false;

    // Используем абсолютную площадь
    float absArea = qAbs(area);

    // Пересчитываем барицентрические координаты правильно
    float s = ((p.y() - c.y()) * (b.x() - c.x()) + (c.y() - b.y()) * (p.x() - c.x())) / (2 * absArea);
    float t = ((a.y() - c.y()) * (p.x() - c.x()) + (c.x() - a.x()) * (p.y() - c.y())) / (2 * absArea);

    qDebug() << "Computed barycentric coordinates:";
    qDebug() << "  s =" << s;
    qDebug() << "  t =" << t;
    qDebug() << "  (s >= 0 && t >= 0 && s + t <= 1):"
             << (s >= 0 && t >= 0 && (s + t) <= 1);

    return s >= 0 && t >= 0 && (s + t) <= 1;
}


QVector<int> triangulatePolygon(
    const QVector<QVector3D>& allVertices,
    const QVector<int>& polygonVertexIndices)
{
    QVector<int> triangles;
    int n = polygonVertexIndices.size();

    if (n < 3) return triangles;

    qDebug() << "---- Initial state ----";
    qDebug() << "Polygon indices:" << polygonVertexIndices;

    QVector<int> indices = polygonVertexIndices; // Работаем с копией
    int current = 0;

    while (n > 3) {
        bool earFound = false;

        for (int i = 0; i < n; ++i) {
            int prevIndex = (i - 1 + n) % n;
            int nextIndex = (i + 1) % n;

            int a = indices[prevIndex];
            int b = indices[i];
            int c = indices[nextIndex];

            QVector3D va = allVertices[a];
            QVector3D vb = allVertices[b];
            QVector3D vc = allVertices[c];

            qDebug() << "\nChecking ear at index" << i << "(vertex" << b << ")";
            qDebug() << "Triangle:" << a << b << c;
            qDebug() << "Coordinates: (" << va << ") (" << vb << ") (" << vc << ")";

            if (isConvex(allVertices, a, b, c)) {
                qDebug() << "Vertex" << b << "is convex";

                bool inside = false;
                for (int j = 0; j < n; ++j) {
                    if (j == prevIndex || j == i || j == nextIndex)
                        continue;

                    QVector3D p = allVertices[indices[j]];
                    if (isInTriangle(p, va, vb, vc)) {
                        qDebug() << "Point" << indices[j] << "inside triangle";
                        inside = true;
                        break;
                    }
                }

                if (!inside) {
                    qDebug() << "Ear found at index" << i << ", vertex" << b;

                    // Добавляем треугольник
                    triangles.append(a);
                    triangles.append(b);
                    triangles.append(c);

                    qDebug() << "Adding triangle:" << a << b << c;

                    // Удаляем вершину из списка
                    indices.remove(i);
                    --n;
                    current = 0;
                    earFound = true;

                    qDebug() << "New indices after removal:" << indices;

                    break;
                } else {
                    qDebug() << "Ear rejected (has inner points)";
                }
            } else {
                qDebug() << "Vertex" << b << "is not convex → skip";
            }
        }

        if (!earFound) {
            qWarning("Could not find an ear. Polygon may be non-simple or degenerate.");
            break;
        }
    }

    if (n == 3) {
        qDebug() << "Final triangle added:" << indices[0] << indices[1] << indices[2];
        triangles.append(indices[0]);
        triangles.append(indices[1]);
        triangles.append(indices[2]);
    }

    qDebug() << "Triangulation complete. Total triangles:" << triangles.size() / 3;
    qDebug() << "Resulting triangles:" << triangles;

    return triangles;
}
