#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui

TARGET = MapShape
TEMPLATE = lib

DEFINES += MAPSHAPE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include($$PWD/../../SoftSDK.pri)

INCLUDEPATH *= $$PWD/../../Inc

DESTDIR = $$PWD/../../../Bin
DLLDESTDIR = $$SDKPath\bin

LIBS *= -L$$DESTDIR

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    DrawShape/SceneCone.h \
    DrawShape/SceneLine.h \
    DrawShape/SceneModel.h \
    DrawShape/ScenePoint.h \
    DrawShape/ScenePolygon.h \
    DrawShape/SceneSCone.h \
    MapShape.h \
    QtMapSceneNode.h

SOURCES += \
    DrawShape/SceneCone.cpp \
    DrawShape/SceneLine.cpp \
    DrawShape/SceneModel.cpp \
    DrawShape/ScenePoint.cpp \
    DrawShape/ScenePolygon.cpp \
    DrawShape/SceneSCone.cpp \
    MapShape.cpp \
    QtMapSceneNode.cpp
