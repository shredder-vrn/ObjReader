#REVIEW: VIEWPORT -> VIEWPORT_PRI
!VIEWPORT {
CONFIG += VIEWPORT

INCLUDEPATH *= $$clean_path($$PWD/../)

#REVIEW: include scene

HEADERS += \
    $$PWD/viewport.h\
    viewportrefactoring.h



SOURCES += \
    $$PWD/viewport.cpp\
    viewportrefactoring.cpp


}







