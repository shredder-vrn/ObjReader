!MAIN_WINDOW {
CONFIG += MAIN_WINDOW

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/mainwindow.h\
    $$PWD/mainwindowrefactoring.h


SOURCES += \
    $$PWD/mainwindow.cpp\
    $$PWD/mainwindowrefactoring.cpp


}


