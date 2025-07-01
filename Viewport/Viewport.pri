#REVIEW: VIEWPORT -> VIEWPORT_PRI
!VIEWPORT {
CONFIG += VIEWPORT

INCLUDEPATH *= $$clean_path($$PWD/../)

#REVIEW: include scene

HEADERS += \
    $$PWD/viewport.h

SOURCES += \
    $$PWD/viewport.cpp

}







