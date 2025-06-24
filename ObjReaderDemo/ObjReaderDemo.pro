QT -= gui
QT += core gui
QT += testlib


CONFIG += c++11 console
CONFIG -= app_bundle


#include($$PWD/../ModelTest/ModelTest.pri)
#include($$PWD/../ModelTest/ModelTest.pro)


SOURCES += \
        $$PWD/../src/obj_reader/objreader.cpp \
        $$PWD/../src/main.cpp



HEADERS += \
        $$PWD/../src/obj_reader/objreader.h \
        $$PWD/../src/obj_reader/modeldata.h

