!VIEWPORT_PRI {
CONFIG += VIEWPORT_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Camera/Camera.pri)
include($$PWD/../Renderer/Renderer.pri)


HEADERS += \
    $$PWD/viewport.h\

SOURCES += \
    $$PWD/viewport.cpp\


}







