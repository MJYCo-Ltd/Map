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

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SDK_CONFIG *= OSG OsgExtern

include(../SoftSDK_My.pri)

INCLUDEPATH *= $$PWD/../Inc

SOURCES += \
    MyNotify.cpp \
    SceneCore.cpp \
    SceneGraph/MyEarthManipulator.cpp \
    SceneGraph/QtRender.cpp \
    SceneGraph/QtSceneGraph.cpp \
    SceneGraph/QtViewPoint.cpp \
    SceneGraph/ResourceLod.cpp \
    SceneGraph/SceneGraphManager.cpp \
    Window/Qml/QtOsgItem.cpp \
    Window/Qml/QtOsgMapItem.cpp \
    Window/Qml/QtOsgUserItem.cpp \
    Window/Qml/SGSimpleTextureNode.cpp \
    Window/QtFBOWindow.cpp \
    Window/QtWindow.cpp \
    Window/QtEventMap.cpp \
    Window/Widget/QtOsgWindow.cpp \
    Message/MessageManager.cpp

HEADERS += \
    ../Inc/Qml/QtOsgMapItem.h \
    ../Inc/Qml/QtOsgItem.h \
    ../Inc/Qml/QtOsgUserItem.h \
    MyNotify.h \
    SceneCore.h \
    SceneGraph/MyEarthManipulator.h \
    SceneGraph/QtRender.h \
    SceneGraph/QtSceneGraph.h \
    SceneGraph/QtViewPoint.h \
    SceneGraph/ResourceLod.h \
    SceneGraph/SceneGraphManager.h \
    Window/Qml/SGSimpleTextureNode.h \
    Window/QtFBOWindow.h \
    Window/QtWindow.h \
    Window/QtEventMap.h \
    Window/Widget/QtOsgWindow.h \
    Message/MessageManager.h

DESTDIR = $$PWD/../../Bin
DLLDESTDIR = $$SDKPath\bin

LIBS *= -L$$DESTDIR
unix {
    target.path = /usr/lib
    INSTALLS += target
}
