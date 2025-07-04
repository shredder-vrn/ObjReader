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

    bool valid = !vertices.isEmpty() &&
                     !faceVertexIndices.isEmpty() &&
                     faceVertexIndices.size() % 3 == 0;

    if (!valid) return false;

    if (hasTexture && textureVertices.isEmpty()) {
        qDebug() << "[WARNING] hasTexture == true, но textureVertices пустые";
        return false;
    }

    if (useNormals && normals.isEmpty()) {
        qDebug() << "[WARNING] useNormal == true, но normals пустые";
        return false;
    }

    return true;
}

bool Model::operator!=(const Model& other) const {
    return faceVertexIndices != other.faceVertexIndices ||
           vertices != other.vertices ||
           polygonStarts != other.polygonStarts;
}
