#include "triangulate.h"
#include <cmath>
#include <QDebug>

bool isInTriangle(const QVector3D& p, const QVector3D& a, const QVector3D& b, const QVector3D& c) {
    // Compute vectors
    QVector3D v0 = c - a;
    QVector3D v1 = b - a;
    QVector3D v2 = p - a;

    // Compute dot products
    float dot00 = QVector3D::dotProduct(v0, v0);
    float dot01 = QVector3D::dotProduct(v0, v1);
    float dot02 = QVector3D::dotProduct(v0, v2);
    float dot11 = QVector3D::dotProduct(v1, v1);
    float dot12 = QVector3D::dotProduct(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    qDebug() << "[isInTriangle] Точка:" << p;
    qDebug() << "Треугольник: A" << a << "B" << b << "C" << c;
    qDebug() << "v0 (C - A):" << v0;
    qDebug() << "v1 (B - A):" << v1;
    qDebug() << "v2 (P - A):" << v2;
    qDebug() << "dot00 (v0·v0):" << dot00;
    qDebug() << "dot01 (v0·v1):" << dot01;
    qDebug() << "dot02 (v0·v2):" << dot02;
    qDebug() << "dot11 (v1·v1):" << dot11;
    qDebug() << "dot12 (v1·v2):" << dot12;
    qDebug() << "invDenom:" << invDenom;
    qDebug() << "u =" << u << "| v =" << v;
    qDebug() << "Результат: точка внутри?" << ((u >= 0) && (v >= 0) && (u + v <= 1));

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool isConvex(const QVector<QVector3D>& allVertices, int prevIndex, int currIndex, int nextIndex) {
    QVector3D prev = allVertices[prevIndex];
    QVector3D curr = allVertices[currIndex];
    QVector3D next = allVertices[nextIndex];

    QVector3D edge1 = curr - prev;
    QVector3D edge2 = next - curr;

    float cross = edge1.x() * edge2.y() - edge1.y() * edge2.x();

    qDebug() << "[isConvex] Проверка выпуклости вершины";
    qDebug() << "Предыдущая вершина:" << prev;
    qDebug() << "Текущая вершина:" << curr;
    qDebug() << "Следующая вершина:" << next;
    qDebug() << "Вектор edge1 (curr - prev):" << edge1;
    qDebug() << "Вектор edge2 (next - curr):" << edge2;
    qDebug() << "Кросс-произведение.z() =" << cross;

    if (qFuzzyIsNull(cross)) {
        qDebug() << "[isConvex] Вершина вырожденная (коллинеарные точки)";
        return false;
    }

    bool isConvexResult = cross > 0;
    qDebug() << "[isConvex] Вершина выпуклая?" << (isConvexResult ? "Да" : "Нет");

    return isConvexResult;
}

bool isPolygonCCW(const QVector<QVector3D>& allVertices, const QVector<int>& polygonVertexIndices) {
    float sum = 0.0f;
    int n = polygonVertexIndices.size();

    for (int i = 0; i < n; ++i) {
        const QVector3D& current = allVertices[polygonVertexIndices[i]];
        const QVector3D& next = allVertices[polygonVertexIndices[(i + 1) % n]];
        sum += (next.x() - current.x()) * (next.y() + current.y());
    }

    bool ccw = sum < 0.0f;

    qDebug() << "[isPolygonCCW] Сумма для определения направления обхода =" << sum;
    qDebug() << "[isPolygonCCW] Направление обхода:" << (ccw ? "Против часовой стрелки (CCW)" : "По часовой стрелке (CW)");

    return ccw;
}

QVector<int> triangulatePolygon(const QVector<QVector3D>& allVertices, const QVector<int>& polygonVertexIndices) {
    QVector<int> triangles;
    QVector<int> indices = polygonVertexIndices;
    int n = indices.size();

    qDebug() << "[triangulatePolygon] === НАЧАЛО ТРИАНГУЛЯЦИИ ===";
    qDebug() << "[triangulatePolygon] Исходные индексы вершин:" << indices;

    qDebug() << "Перед началом все вершины:";
    for (int i = 0; i < indices.size(); ++i) {
        qDebug() << "  Вершина" << indices[i] << "=" << allVertices[indices[i]];
    }

    if (n < 3) {
        qDebug() << "[triangulatePolygon] ❌ Полигон невалиден: меньше 3 вершин";
        return triangles;
    }

    // Убедимся, что полигон задан против часовой стрелки
    if (!isPolygonCCW(allVertices, indices)) {
        qDebug() << "[triangulatePolygon] 🔄 Меняем направление обхода на противоположное";
        std::reverse(indices.begin(), indices.end());
    }

    // Площадь полигона для проверки вырожденности
    float area = 0.0f;
    for (int i = 0; i < n; ++i) {
        const QVector3D& current = allVertices[indices[i]];
        const QVector3D& next = allVertices[indices[(i + 1) % n]];
        area += current.x() * next.y() - next.x() * current.y();
    }
    area = fabs(area) / 2.0f;

    qDebug() << "[triangulatePolygon] Вычисленная площадь полигона:" << area;

    if (qFuzzyIsNull(area)) {
        qDebug() << "[triangulatePolygon] ⚠️ Полигон вырожденный (площадь ≈ 0)";
        return triangles;
    }

    while (n > 3) {
        bool earFound = false;

        qDebug() << "\n[triangulatePolygon] 🔍 Поиск уха в полигоне из" << n << "вершин";

        for (int i = 0; i < n && !earFound; ++i) {
            int prevIndex = (i - 1 + n) % n;
            int currIndex = i;
            int nextIndex = (i + 1) % n;

            int a = indices[prevIndex];
            int b = indices[currIndex];
            int c = indices[nextIndex];

            QVector3D va = allVertices[a];
            QVector3D vb = allVertices[b];
            QVector3D vc = allVertices[c];

            qDebug() << "[triangulatePolygon] Проверка вершины" << i << "(индекс" << b << ")";
            qDebug() << "Треугольник: A" << a << "=" << va;
            qDebug() << "             B" << b << "=" << vb;
            qDebug() << "             C" << c << "=" << vc;

            if (isConvex(allVertices, a, b, c)) {
                bool isEar = true;
                QVector3D va = allVertices[a];
                QVector3D vb = allVertices[b];
                QVector3D vc = allVertices[c];

                qDebug() << "[triangulatePolygon] ✅ Вершина" << b << "выпуклая → возможное 'ушко'";

                for (int j = 0; j < n && isEar; ++j) {
                    if (j == prevIndex || j == currIndex || j == nextIndex)
                        continue;

                    QVector3D p = allVertices[indices[j]];

                    qDebug() << "[triangulatePolygon] Проверка точки" << indices[j] << "(" << p << ") внутри треугольника";

                    if (isInTriangle(p, va, vb, vc)) {
                        qDebug() << "[triangulatePolygon] ❌ Точка" << indices[j] << "внутри треугольника → не ушко";
                        isEar = false;
                    }
                }

                if (isEar) {
                    qDebug() << "[triangulatePolygon] ✅ Ушко найдено: вершины" << a << b << c;
                    triangles.append(a);
                    triangles.append(b);
                    triangles.append(c);

                    qDebug() << "[triangulatePolygon] 🗑 Удаление вершины" << b;
                    indices.removeAt(currIndex);
                    --n;
                    earFound = true;
                } else {
                    qDebug() << "[triangulatePolygon] ❌ Ушко не найдено на этой итерации";
                }
            } else {
                qDebug() << "[triangulatePolygon] ❌ Вершина" << b << "не выпуклая → пропуск";
            }
        }

        if (!earFound) {
            qWarning("[triangulatePolygon] ⚠️ Не удалось найти ушко → остановка");
            break;
        }
    }

    if (n == 3) {
        qDebug() << "[triangulatePolygon] 🏁 Последний треугольник добавлен:" << indices[0] << indices[1] << indices[2];
        triangles.append(indices[0]);
        triangles.append(indices[1]);
        triangles.append(indices[2]);
    }

    qDebug() << "[triangulatePolygon] ✅ Триангуляция завершена";
    qDebug() << "           Количество треугольников:" << triangles.size() / 3;
    qDebug() << "           Индексы треугольников:" << triangles;

    return triangles;
}
