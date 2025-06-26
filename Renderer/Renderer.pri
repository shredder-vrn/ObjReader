!RENDER {
CONFIG += RENDER

INCLUDEPATH *= $$clean_path($$PWD/../)

#include($$PWD/../Model/Model.pri)
#include($$PWD/../ObjReader/ObjReader.pri)

SOURCES += \
    $$PWD/renderer.cpp

HEADERS += \
    $$PWD/renderer.h
}












