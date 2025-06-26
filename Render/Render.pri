!RENDER {
CONFIG += RENDER

INCLUDEPATH *= $$clean_path($$PWD/../)

#include($$PWD/../Model/Model.pri)
#include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    #$$PWD/mesh.h \
    #$$PWD/vertex.h

}

SOURCES += \
   # $$PWD/mesh.cpp










