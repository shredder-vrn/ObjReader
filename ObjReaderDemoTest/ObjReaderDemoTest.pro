QT -= gui
QT += core gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

include($$PWD/../ObjReaderDemoTest/ObjReaderDemoTest.pri)

SOURCES += \
    $$PWD/main.cpp
