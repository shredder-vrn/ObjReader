!RENDER_PRI {
CONFIG += RENDER_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Shaders/Shaders.pri)

HEADERS += \
    $$PWD/renderer.h\

SOURCES += \
    $$PWD/renderer.cpp\

}












