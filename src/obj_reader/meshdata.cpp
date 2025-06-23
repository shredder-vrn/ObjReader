#include "meshdata.h"
#include <QDebug>

namespace MeshData{

bool checkVertices(const Model &model)
{
    if (model.vertices.isEmpty()){
        qInfo("[Ошибка!] Нет вершин: модель не может быть отрисована.");
        return false;
    }
    return true;
}

bool checkFaces(const Model &model){
    if (model.faces.isEmpty()){
        qInfo("Предупреждение!] Нет граней: каркасная отрисовка невозможна.");
        return false;
    }

    for (const Face &face:model.faces){
        for (int idx:face.vertexIndices){
            if (idx < 0 || idx >= model.vertices.size()){
                qInfo("[Ошибка!] vertexIndex выходит за границы:");
                return false;
            }
        }
    }

    return true;
}

bool checkTexCords(const Model &model){
    if (model.texCoords.isEmpty()){
        qInfo("[Предупреждение!] Нет текстурных координат: текстурирование невозможно.");
        return false;
    }

    bool allFacesHaveValidTexCoord = true;

    for (const Face &face : model.faces){
        qDebug() << face.texCoordIndices;
        if (!face.texCoordIndices.isEmpty()){
            if (face.texCoordIndices.size() != face.vertexIndices.size()){
                qInfo("[Ошибка!] Несоответствие количества текстурных координат и вершин в грани.");
                allFacesHaveValidTexCoord = false;
            }

            for (int idx : face.texCoordIndices){
                if (idx < 0 || idx >= model.texCoords.size()) {
                    qInfo("[Ошибка!] texCoords выходит за границы.");
                    allFacesHaveValidTexCoord = false;
                }

            }
        }

    }

    if (!allFacesHaveValidTexCoord){
        qInfo("[Ошибка!] Данные повреждены или неполные: текстурирование невозможно.");
        return false;
    }

    qInfo("всё ок");
    return true;
}

bool checkNormals(const Model &model){
    if (model.normals.isEmpty()){
        qInfo("[Предупреждение!] Нет нормалей: освещение невозможно.");
        return false;
    }

    bool allFacesHaveValidNormals = true;

    for (const Face &face : model.faces){
        if (!face.normalIndices.isEmpty()){
            if (face.normalIndices.size() != face.vertexIndices.size()){
                qInfo("[Ошибка!] Несоответствие количества нормалей и вершин в грани.");
                allFacesHaveValidNormals = false;
            }

            for (int idx : face.normalIndices){
                if (idx < 0 || idx >= model.normals.size()) {
                    qInfo("[Ошибка!] нормаль выходит за границы.");
                    allFacesHaveValidNormals = false;
                }

            }
        }

    }

    if (!allFacesHaveValidNormals){
        qInfo("[Ошибка!] Данные повреждены или неполные: освещение невозможно.");
        return false;
    }

    return true;}

}


bool validateModel(const Model &model){
    bool hasVertexData = MeshData::checkVertices(model);
    bool hasFaceData = MeshData::checkFaces(model);
    bool hasTexCoorData = MeshData::checkTexCords(model);
    bool hasNormalData = MeshData::checkNormals(model);

    bool canDrawWireframe = hasVertexData && hasFaceData;
    bool canDrawWithTextures = canDrawWireframe && hasTexCoorData;
    bool canDrawWithLighting = canDrawWireframe && hasNormalData;
    bool canDrawFullmodel = canDrawWithTextures && canDrawWithLighting;

    qWarning() << "Режим: 'Каркас'" << (canDrawWireframe ? "Поддерживается":"Не поддерживается");
    qWarning() << "Режим: 'Текстуры'" << (canDrawWithTextures ? "Поддерживается":"Не поддерживается");
    qWarning() << "Режим: 'Освещение'" << (canDrawWithLighting ? "Поддерживается":"Не поддерживается");
    qWarning() << "Режим: 'Модель'" << (canDrawFullmodel ? "Поддерживается":"Не поддерживается");

    return canDrawWireframe;
}
