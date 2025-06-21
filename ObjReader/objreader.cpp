#include "objreader.h"
#include "Model/model.h"

#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

bool parseObjvertices(const QString &filePath, Model &model)
{

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Не удалось открыть текст" << filePath;
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd()){
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);

        if (tokens.isEmpty())
            continue;

        QString type = tokens[0];

        if (type == "#"){
            continue;
        }
        else if (type == "v" && tokens.size() >= 4){
            bool okX = false;
            bool okY = false;
            bool okZ = false;
            float x = tokens[1].toFloat(&okX);
            float y = tokens[2].toFloat(&okY);
            float z = tokens[3].toFloat(&okZ);
            if (okX && okY && okZ){
                model.vertices.append(QVector3D(x, y, z));
                continue;
            }
        }
        else if (type == "vt" && tokens.size() >= 3){
            bool okU = false;
            bool okV = false;
            float u = tokens[1].toFloat(&okU);
            float v = tokens[2].toFloat(&okV);
            if (okU && okV){
                model.texCoords.append(QVector2D(u, v));
                continue;
            }
        }
        else if (type == "vn"){
            if (tokens.size() >= 4){
                bool okX = false;
                bool okY = false;
                bool okZ = false;
                float x = tokens[1].toFloat(&okX);
                float y = tokens[2].toFloat(&okY);
                float z = tokens[3].toFloat(&okZ);
                if (okX && okY && okZ){
                    model.normals.append(QVector3D(x, y, z));
                    continue;
                }
            } else {

            }
        }
        else if (type == "f"){
            Face face;
            for (int i = 1; i < tokens.size(); ++i){
                QStringList parts = tokens[i].split('/');
                if (parts.size() >= 1){
                    bool ok = false;
                    int vertexIndex = parts[0].toInt(&ok);
                    if (ok) {
                        if (vertexIndex > 0) {
                            face.vertexIndices.append(vertexIndex-1);
                        } else {

                        }
                    }
                } else {

                }
                if (parts.size() >= 2){
                    bool ok = false;
                    int texCoordIndex = parts[1].toInt(&ok);
                    if (ok && texCoordIndex > 0){
                        face.texCoordIndices.append(texCoordIndex-1);
                        qDebug() << texCoordIndex-1;
                    }
                } else {

                }
                if (parts.size() >= 3){
                    bool ok = false;
                    int normalIndex = parts[2].toInt(&ok);
                    if (ok) {
                        if (normalIndex > 0) {
                            face.normalIndices.append(normalIndex-1);
                            qDebug() << normalIndex-1;
                        } else {

                        }
                    }
                }else{

                }
            }

            Q_ASSERT_X(face.vertexIndices.size() >= 3, "", "");

            if (!face.vertexIndices.isEmpty()){
                model.faces.append(face);
            } else {
                qWarning() << "Пропущена грань, не достаточно вершин";
            }
        }
    }
    return true;
}

