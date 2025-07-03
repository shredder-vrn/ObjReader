!RENDER_PRI {
CONFIG += RENDER_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

#include($$PWD/../Model/Model.pri)
#include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    $$PWD/renderer.h\
#    $$PWD/rendererrefactoring.h

SOURCES += \
    $$PWD/renderer.cpp\
#    $$PWD/rendererrefactoring.cpp
}












