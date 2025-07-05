!VERTEX_SHADER {

CONFIG += VERTEX_SHADER

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../VertexShader/VertexShader.pri)
include($$PWD/../FragmentShader/FragmentShader.pri)


SOURCES += \
    $$PWD/shadersprogram.cpp

HEADERS += \
    $$PWD/shadersprogram.h


}













