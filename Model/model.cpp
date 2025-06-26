#include "model.h"

void Model::clear() {
    vertices.clear();
    textureVertices.clear();
    normals.clear();

    faceVertexIndices.clear();
    faceTextureVertexIndices.clear();
    faceNormalIndices.clear();

    polygonStarts.clear();
}

bool Model::isValid() const {
    return !vertices.isEmpty() && !faceVertexIndices.isEmpty();
}
