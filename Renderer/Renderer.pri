!RENDER_PRI {
CONFIG += RENDER_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

#include($$PWD/../Model/Model.pri)
#include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    $$PWD/renderer.h\
    $$PWD/glmodel.h

#    $$PWD/rendererrefactoring.h

SOURCES += \
    $$PWD/renderer.cpp\
    $$PWD/glmodel.cpp

#    $$PWD/rendererrefactoring.cpp
}












