#include "model.h"

void Model::clear() {
<<<<<<< Updated upstream
=======
    qDebug() << "Model :: clear : запустили метод clear";

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
    qDebug() << "Model :: isValid : запустили метод isValid";

>>>>>>> Stashed changes
    return !vertices.isEmpty() && !faceVertexIndices.isEmpty();

    qDebug() << "Model :: isValid : метод isValid отработал";
}
