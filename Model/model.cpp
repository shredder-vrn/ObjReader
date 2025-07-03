#include "model.h"

#include <QDebug>

void Model::clear() {

    qDebug() << "Model :: clear : запустили метод clear";

    vertices.clear();
    textureVertices.clear();
    normals.clear();

    faceVertexIndices.clear();
    faceTextureVertexIndices.clear();
    faceNormalIndices.clear();

    polygonStarts.clear();
    qDebug() << "Model :: clear : метод clear отработал";
}

bool Model::isValid() const {

    qDebug() << "Model :: isValid : запустили метод isValid";

    return !vertices.isEmpty() && !faceVertexIndices.isEmpty();

    qDebug() << "Model :: isValid : метод isValid отработал";
}

bool Model::operator!=(const Model& other) const {
    return faceVertexIndices != other.faceVertexIndices ||
           vertices != other.vertices ||
           polygonStarts != other.polygonStarts;
}
