#include <QVector>
#include <QVector3D>
#include <QList>
#include <QDebug>
#include <cmath>

void simpleTriangulateModel(
    const QVector<QVector3D> &vertices,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTextureVertexIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts)
{
    QVector<int> newFaceVertexIndices;
    QVector<int> newFaceTextureVertexIndices;
    QVector<int> newFaceNormalIndices;
    QVector<int> newPolygonStarts;

    qDebug() << "vertices" << vertices;
    qDebug() << "faceVertexIndices" << faceVertexIndices;
    qDebug() << "faceTextureVertexIndices" << faceTextureVertexIndices;
    qDebug() << "faceNormalIndices" << faceNormalIndices;
    qDebug() << "polygonStarts" << polygonStarts;

    int faceCount = polygonStarts.size();

    for (int i = 0; i < faceCount; ++i) {
        int start = polygonStarts[i];
        int end = (i + 1 < faceCount) ? polygonStarts[i + 1] : faceVertexIndices.size();

        int vertexCount = end - start;
        if (vertexCount != 4) {
            qWarning() << "Поддерживаются только четырёхугольники!";
            continue;
        }

        // Сохраняем начало этой грани в массиве новых индексов
        newPolygonStarts.append(newFaceVertexIndices.size());
        newPolygonStarts.append(newFaceVertexIndices.size() + 3);

        // Индексы вершин четырёхугольника
        int a = faceVertexIndices[start + 0];
        int b = faceVertexIndices[start + 1];
        int c = faceVertexIndices[start + 2];
        int d = faceVertexIndices[start + 3];

        // Первый треугольник: A -> B -> C
        newFaceVertexIndices.append(a);
        newFaceVertexIndices.append(b);
        newFaceVertexIndices.append(c);

        // Второй треугольник: A -> C -> D
        newFaceVertexIndices.append(a);
        newFaceVertexIndices.append(c);
        newFaceVertexIndices.append(d);

        // Обрабатываем текстурные координаты
        if (!faceTextureVertexIndices.isEmpty()) {
            int ta = faceTextureVertexIndices[start + 0];
            int tb = faceTextureVertexIndices[start + 1];
            int tc = faceTextureVertexIndices[start + 2];
            int td = faceTextureVertexIndices[start + 3];

            newFaceTextureVertexIndices.append(ta);
            newFaceTextureVertexIndices.append(tb);
            newFaceTextureVertexIndices.append(tc);

            newFaceTextureVertexIndices.append(ta);
            newFaceTextureVertexIndices.append(tc);
            newFaceTextureVertexIndices.append(td);
        }

        // Обрабатываем нормали
        if (!faceNormalIndices.isEmpty()) {
            int na = faceNormalIndices[start + 0];
            int nb = faceNormalIndices[start + 1];
            int nc = faceNormalIndices[start + 2];
            int nd = faceNormalIndices[start + 3];

            newFaceNormalIndices.append(na);
            newFaceNormalIndices.append(nb);
            newFaceNormalIndices.append(nc);

            newFaceNormalIndices.append(na);
            newFaceNormalIndices.append(nc);
            newFaceNormalIndices.append(nd);
        }
    }

    // Перезаписываем старые данные новыми
    faceVertexIndices = newFaceVertexIndices;
    faceTextureVertexIndices = newFaceTextureVertexIndices;
    faceNormalIndices = newFaceNormalIndices;
    polygonStarts = newPolygonStarts;

    qDebug() << "vertices" << vertices;
    qDebug() << "newFaceVertexIndices" << faceVertexIndices;
    qDebug() << "newPolygonStarts" << polygonStarts;
}


bool pointsEqual(const QVector3D &a, const QVector3D &b, float epsilon = 1e-5f) {
    Q_UNUSED(epsilon);
    return qFuzzyCompare(a, b);
}

bool isConvex(
    const QVector3D &prev,
    const QVector3D &curr,
    const QVector3D &next,
    const QVector<QVector3D> &polygonPoints)
{
    QVector3D edge1 = curr - prev;
    QVector3D edge2 = next - curr;

    QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();
    QVector3D polygonNormal(0, 0, 0);

    if (polygonPoints.size() >= 3) {
        QVector3D v1 = polygonPoints[1] - polygonPoints[0];
        QVector3D v2 = polygonPoints[2] - polygonPoints[0];
        polygonNormal = QVector3D::crossProduct(v1, v2).normalized();
    }

    float dot = QVector3D::dotProduct(normal, polygonNormal);
    return dot > 0;
}

bool isEar(
    const QVector3D &a,
    const QVector3D &b,
    const QVector3D &c,
    const QVector<QVector3D> &polygonPoints)
{
    if (!isConvex(a, b, c, polygonPoints)) return false;

    for (const QVector3D &p : polygonPoints) {
        if (pointsEqual(p, a) || pointsEqual(p, b) || pointsEqual(p, c))
            continue;

        auto sameSide = [](const QVector3D &p1, const QVector3D &p2,
                           const QVector3D &a, const QVector3D &b) {
            QVector3D cp1 = QVector3D::crossProduct(b - a, p1 - a);
            QVector3D cp2 = QVector3D::crossProduct(b - a, p2 - a);
            return QVector3D::dotProduct(cp1, cp2) >= 0;
        };

        bool s1 = sameSide(p, a, b, c);
        bool s2 = sameSide(p, b, a, c);
        bool s3 = sameSide(p, c, a, b);

        if (s1 && s2 && s3) return false;
    }

    return true;
}

void triangulateModel(
    const QVector<QVector3D> &vertices,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTextureVertexIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts)
{
    qDebug() << "triangulateModel :: triangulateModel : запустили метод triangulateModel";

    QVector<int> newFaceVertexIndices;
    QVector<int> newFaceTextureVertexIndices;
    QVector<int> newFaceNormalIndices;
    QVector<int> newPolygonStarts;


    int faceCount = polygonStarts.size();
    for (int i = 0; i < faceCount; ++i) {
        int start = polygonStarts[i];
        int end = (i + 1 < faceCount) ? polygonStarts[i + 1] : faceVertexIndices.size();

        int vertexCount = end - start;
        if (vertexCount < 3) continue;

        QList<int> indices;
        for (int j = start; j < end; ++j) {
            indices.append(faceVertexIndices[j]);
        }

        QVector<QVector3D> polygonPoints;
        for (int idx : indices) {
            polygonPoints.append(vertices[idx]);
        }

        newPolygonStarts.append(newFaceVertexIndices.size());

        while (indices.size() > 3) {
            bool foundEar = false;
            for (int iLoop = 0; iLoop < indices.size(); ++iLoop) {
                int prevIdx = indices[(iLoop - 1 + indices.size()) % indices.size()];
                int currIdx = indices[iLoop];
                int nextIdx = indices[(iLoop + 1) % indices.size()];

                QVector3D prevPt = vertices[prevIdx];
                QVector3D currPt = vertices[currIdx];
                QVector3D nextPt = vertices[nextIdx];

                if (isEar(prevPt, currPt, nextPt, polygonPoints)) {
                    newFaceVertexIndices.append(prevIdx);
                    newFaceVertexIndices.append(currIdx);
                    newFaceVertexIndices.append(nextIdx);

                    newPolygonStarts.append(newFaceVertexIndices.size());


                    if (!faceTextureVertexIndices.isEmpty()) {
                        newFaceTextureVertexIndices.append(faceTextureVertexIndices[prevIdx]);
                        newFaceTextureVertexIndices.append(faceTextureVertexIndices[currIdx]);
                        newFaceTextureVertexIndices.append(faceTextureVertexIndices[nextIdx]);
                    }

                    if (!faceNormalIndices.isEmpty()) {
                        newFaceNormalIndices.append(faceNormalIndices[prevIdx]);
                        newFaceNormalIndices.append(faceNormalIndices[currIdx]);
                        newFaceNormalIndices.append(faceNormalIndices[nextIdx]);
                    }

                    indices.removeAt(iLoop);
                    foundEar = true;
                    break;
                }
            }

            if (!foundEar) {
                break;
            }
        }

        if (indices.size() == 3) {
            newFaceVertexIndices.append(indices[0]);
            newFaceVertexIndices.append(indices[1]);
            newFaceVertexIndices.append(indices[2]);

            if (!faceTextureVertexIndices.isEmpty()) {
                newFaceTextureVertexIndices.append(faceTextureVertexIndices[indices[0]]);
                newFaceTextureVertexIndices.append(faceTextureVertexIndices[indices[1]]);
                newFaceTextureVertexIndices.append(faceTextureVertexIndices[indices[2]]);
            }

            if (!faceNormalIndices.isEmpty()) {
                newFaceNormalIndices.append(faceNormalIndices[indices[0]]);
                newFaceNormalIndices.append(faceNormalIndices[indices[1]]);
                newFaceNormalIndices.append(faceNormalIndices[indices[2]]);
            }
        }
    }

    faceVertexIndices = newFaceVertexIndices;
    faceTextureVertexIndices = newFaceTextureVertexIndices;
    faceNormalIndices = newFaceNormalIndices;
    polygonStarts = newPolygonStarts;

    qDebug() << "vertices" << vertices;
    qDebug() << "faceVertexIndices" << faceVertexIndices;
    qDebug() << "faceTextureVertexIndices" << faceTextureVertexIndices;
    qDebug() << "faceNormalIndices" << faceNormalIndices;
    qDebug() << "polygonStarts" << polygonStarts;
    qDebug() << "triangulateModel :: triangulateModel : метод triangulateModel отработал";

}
