QT -= gui
QT += core gui
QT += testlib
QT += core gui widgets opengl

CONFIG += c++11 console
CONFIG -= app_bundle

include($$PWD/../Model/Model.pri)
include($$PWD/../ObjReader/ObjReader.pri)
include($$PWD/../Viewport/Viewport.pri)
include($$PWD/../Render/Render.pri)
include($$PWD/../Triangulate/Triangulate.pri)

SOURCES += \
        $$PWD/main.cpp \
