#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui
QT += widgets quick

TARGET = SceneCore
TEMPLATE = lib

DEFINES += SCENECORE_LIBRARY

SDK_CONFIG *= OSG OsgExtern

include(../SoftSDK.pri)

SOURCES += \
    MyNotify.cpp \
    SceneCore.cpp \
    SceneGraph/MyEarthManipulator.cpp \
    SceneGraph/QtRender.cpp \
    SceneGraph/QtSceneGraph.cpp \
    SceneGraph/QtViewHud.cpp \
    SceneGraph/QtViewPort.cpp \
    SceneGraph/ResourceLod.cpp \
    SceneGraph/SceneGraphManager.cpp \
    Window/Qml/QtOsgItem.cpp \
    Window/Qml/QtOsgRenderer.cpp \
    Window/QtFBOWindow.cpp \
    Window/QtWindow.cpp \
    Window/QtEventMap.cpp \
    Window/Widget/QtOsgWindow.cpp

HEADERS += \
    ../Inc/Qml/QtOsgItem.h \
    MyNotify.h \
    SceneCore.h \
    SceneGraph/DealViewPortChange.h \
    SceneGraph/MyEarthManipulator.h \
    SceneGraph/QtRender.h \
    SceneGraph/QtSceneGraph.h \
    SceneGraph/QtViewHud.h \
    SceneGraph/QtViewPort.h \
    SceneGraph/ResourceLod.h \
    SceneGraph/SceneGraphManager.h \
    SceneGraph/ViewPortEventCallback.h \
    Window/MyShader.h \
    Window/Qml/QtOsgRenderer.h \
    Window/QtFBOWindow.h \
    Window/QtWindow.h \
    Window/QtEventMap.h \
    Window/Widget/QtOsgWindow.h
