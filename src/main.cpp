#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>


#include "../src/obj_reader/meshdata.h"
#include "../src/obj_reader/objreader.h"


void printStats(Model &model)
{
    {
        qDebug() << "|----------------------------------------------|";
        qDebug() << "Вершин: " << model.vertices.size();
        qDebug() << "Текстурных Вершин: " << model.texCoords.size();
        qDebug() << "Нормалей: " << model.normals.size();
        qDebug() << "Полигонов: " << model.faces.size();
        qDebug() << "|----------------------------------------------|";
    }

    for (int i = 0; i < model.vertices.size(); ++i){
        qDebug() << "v" << i+1 << ":" << model.vertices[i];
    }


    for (int i = 0; i < model.texCoords.size(); ++i){
        qDebug() << "vt" << i+1 << ":" << model.texCoords[i];
    }

    for (int i = 0; i < model.normals.size(); ++i){
        qDebug() << "vn" << i+1 << ":" << model.normals[i];
    }

    for (int i = 0; i < model.faces.size(); ++i){
        qDebug() << "f" << i+1 << ":"
                 << "v" << model.faces[i].vertexIndices
                 << "vt" << model.faces[i].texCoordIndices
                 << "vn" << model.faces[i].normalIndices;
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Model model;

    QString filename = "/home/r3ds/Internship/resources/cube.obj";

    if (!parseObjvertices(filename, model)) {
        qCritical() << "Ошибка при запуске файла";
        return 1;
    }

    Q_ASSERT(validateModel(model));

    printStats(model);

    return 0;

}



/*
        // !REVIEW: проверки на валидность данных
        if (type == "v"){
            // !REVIEW: Vertex -> QVector3D
            Vertex v;
            if (iss>> v.x >> v.y >> v.z){
                vertices.push_back(v);
            }
        } else if (type == "vt"){
            TexCoord t;
            // !REVIEW: TexCoord -> QVector2D
            // !REVIEW: может быть 3 координаты
            if (iss>>t.u>>t.v){
                texCoords.push_back(t);
            }
        } else if (type == "vn"){
            Normal n;
            // !REVIEW: нормали не читаем
            if (iss>>n.x>>n.y>>n.z){
                normals.push_back(n);
            }
        } else if (type == "f") {
            Face f;
            // !REVIEW: может быть много вершин у полигона
            // !REVIEW: может не быть текстурных или нормальных координат
            // 1/1 1//1 1
            char slash;
            for (size_t i = 0; i < 3; ++i){
                //iss >> f.vertexIndex[i] >> slash >> f.texCoordIndex[i] >> slash >> f.normalIndex[i];
            }
            faces.push_back(f);
        }


    for (size_t i = 0; i < vertices.size(); ++i) {
        //! REVIEW:
        // const auto& v = vertices[i];
        // const QVector3D& v = vertices[i];

        // const auto v = QVector3D(1, 2, 3);
        //! REVIEW:
        // const QVector3D &v = vertices[i];
        // int& a, b;
        // int &a, b;
        const auto &v = vertices[i];
        std::cout<<"v: "<<v.x<<" "<<v.y<<" "<<v.z<<std::endl;
    }



void runTests()
{
    std::cout<<"|----------------------------------------------|"<<std::endl;
    QVector<QVector3D> vertices;
    //std::vector<Vertex> vertices;
    std::string testData =
            "v 1.0 2.0 3.0\n"
            "v 4.0 5.0 6.0\n"
            "v 7.0 8.0 9.0\n";
    std::cout<<testData<<std::endl;

    //parseObjvertices(testData, vertices);
    // #include <QtGlobal>
//    Q_ASSERT_X(vertices.size() == 2, "Должно быть 3 вершины");
    //assert(vertices.size() == 3 && "Должно быть 3 вершины");
}*/

/*
//! REVIEW: QVector3D
//! REVIEW:
struct Vertex
{
//    float x, y, z;
    //! REVIEW:
    float x = 0;
    float y = 0;
    float z = 0;
};

struct TexCoord
{
    float u = 0;
    float v = 0;
};

struct Normal
{
    float x = 0;
    float y = 0;
    float z = 0;
};

//! REVIEW:
//! std::vector<std::vector<int>> polygonVertexIndices;
//! std::vector<std::vector<int>> polygonTextureVertexIndices;
//! f 1 2 3
//! f 1 2 4
//! f 5 6 7 8
//! polygonVertexIndices = {
//!     {1, 2, 3},
//!     {1, 2, 4},
//!     {5, 6, 7, 8}
//! }
//!
//!     //! REVIEW: unsigned int -> int
    //! unsigned int vertexIndex[3] = {0, 0, 0};
//!*/
