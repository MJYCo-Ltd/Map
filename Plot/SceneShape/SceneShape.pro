#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = SceneShape
TEMPLATE = lib

SDK_CONFIG *= OSG Satellite OsgExtern PLOT

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    DrawShape/SceneBox.h \
    DrawShape/SceneCone.h \
    DrawShape/SceneEllipsoid.h \
    DrawShape/SceneImage.h \
    DrawShape/SceneLabel.h \
    DrawShape/SceneLine.h \
    DrawShape/ScenePoint.h \
    DrawShape/ScenePolygon.h \
    DrawShape/SceneSCone.h \
    SceneShape.h

SOURCES += \
    DrawShape/SceneBox.cpp \
    DrawShape/SceneCone.cpp \
    DrawShape/SceneEllipsoid.cpp \
    DrawShape/SceneImage.cpp \
    DrawShape/SceneLabel.cpp \
    DrawShape/SceneLine.cpp \
    DrawShape/ScenePoint.cpp \
    DrawShape/ScenePolygon.cpp \
    DrawShape/SceneSCone.cpp \
    SceneShape.cpp
