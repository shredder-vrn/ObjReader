!MODEL_CONTROLLER_PRI {
CONFIG += MODEL_CONTROLLER_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Triangulate/Triangulate.pri)
include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    $$PWD/modelcontroller.h

SOURCES += \
    $$PWD/modelcontroller.cpp

}






