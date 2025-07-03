!CAMERA_PRI {

CONFIG += CAMERA_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
#    $$PWD/camera.h\
    $$PWD/cameraort.h \
    $$PWD/cameraper.h\
#    $$PWD/camerarefactoring.h \
    $$PWD/сamerainterface.h


SOURCES += \
    $$PWD/camera.cpp\
    $$PWD/cameraort.cpp \
    $$PWD/cameraper.cpp \
#    $$PWD/camerarefactoring.cpp\
   # $$PWD/сameraInterface.cpp


}

