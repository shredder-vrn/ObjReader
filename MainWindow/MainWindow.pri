!MAIN_WINDOW_PRI {
CONFIG += MAIN_WINDOW_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Model/Model.pri)
include($$PWD/../ObjectsGL/ObjectsGL.pri)
include($$PWD/../Viewport/Viewport.pri)
include($$PWD/../Triangulate/Triangulate.pri)
include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    $$PWD/mainwindow.h\

SOURCES += \
    $$PWD/mainwindow.cpp\

}


