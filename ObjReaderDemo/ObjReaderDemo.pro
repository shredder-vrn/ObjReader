QT -= gui
QT += core gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include($$PWD/../ObjReader/ObjReader.pri)
#include($$PWD/../ObjReaderTest/ObjReaderTest.pri)
#include($$PWD/../ModelTest/ModelTest.pri)
#include($$PWD/../ModelTest/ModelTest.pro)

# include($$PWD/../Model/Model.pri)
# include($$PWD/../ModelTest/ModelTest.pri)


SOURCES += \
#        $$PWD/../src/main.cpp \
        $$PWD/../src/obj_reader/objreader.cpp \
        $$PWD/../src/obj_reader/meshdata.cpp



HEADERS += \
        $$PWD/../src/obj_reader/objreader.h \
        $$PWD/../src/obj_reader/meshdata.h

