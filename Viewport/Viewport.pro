LIBS += -lGL
QT -= gui
QT += core gui
QT += testlib
QT += core gui widgets opengl
QT += opengl
QMAKE_CXXFLAGS += -std=c++17

CONFIG += c++17 console
CONFIG -= app_bundle

include($$PWD/../MainWindow/MainWindow.pri)
include($$PWD/../Viewport/Viewport.pri)
include($$PWD/../Model/Model.pri)
include($$PWD/../Scene/Scene.pri)
include($$PWD/../ModelController/ModelController.pri)
include($$PWD/../Renderer/Renderer.pri)
include($$PWD/../ObjReader/ObjReader.pri)
include($$PWD/../Shaders/Shaders.pri)
include($$PWD/../Triangulate/Triangulate.pri)
include($$PWD/../Camera/Camera.pri)


SOURCES += \
        $$PWD/main.cpp \
