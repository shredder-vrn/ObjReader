#include "meshdata.h"


bool validateModel(const Model &model){
    Q_ASSERT_X(!model.vertices.isEmpty(), "Model", "Модель не содержит вершин.");
    //Q_ASSERT_X(model.faces.size() > 0, "Model", "Модель не содержит граней.");

    for (int i = 0; i < model.faces.size(); ++i){
        const Face &face = model.faces[i];

        //Q_ASSERT_X(face.vertexIndices.size() >= 3, "", "");

        //Q_ASSERT_X(face.texCoordIndices.isEmpty() || face.texCoordIndices.size() == face.vertexIndices.size(),"","");

        //Q_ASSERT_X(face.normalIndices.isEmpty() || face.normalIndices.size() == face.vertexIndices.size(),"","");

        for (int idx : face.vertexIndices){
            //Q_ASSERT_X(idx >= 0 && idx < model.vertices.size(), "", "");
            //Q_ASSERT_X(idx >= 0 && idx < model.texCoords.size(), "", "");
            //Q_ASSERT_X(idx >= 0 && idx < model.normals.size(), "", "");
        }
    }

    return true;
}
