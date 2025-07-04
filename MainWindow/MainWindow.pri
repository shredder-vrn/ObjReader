!MAIN_WINDOW_PRI {
CONFIG += MAIN_WINDOW_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../ModelController/ModelController.pri)
include($$PWD/../Model/Model.pri)
include($$PWD/../Viewport/Viewport.pri)

HEADERS += \
    $$PWD/mainwindow.h\

SOURCES += \
    $$PWD/mainwindow.cpp\

}


