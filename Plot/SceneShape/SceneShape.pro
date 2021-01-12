#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = SceneShape
TEMPLATE = lib

SDK_CONFIG *= OSG Satellite OsgExtern
include($$PWD/../MapPlot.pri)

HEADERS += \
    DrawShape/SceneCone.h \
    DrawShape/SceneImage.h \
    DrawShape/SceneLabel.h \
    DrawShape/SceneLine.h \
    DrawShape/ScenePoint.h \
    DrawShape/ScenePolygon.h \
    DrawShape/ScenePulse.h \
    DrawShape/SceneRadar.h \
    DrawShape/SceneSCone.h \
    SceneShape.h

SOURCES += \
    DrawShape/SceneCone.cpp \
    DrawShape/SceneImage.cpp \
    DrawShape/SceneLabel.cpp \
    DrawShape/SceneLine.cpp \
    DrawShape/ScenePoint.cpp \
    DrawShape/ScenePolygon.cpp \
    DrawShape/ScenePulse.cpp \
    DrawShape/SceneRadar.cpp \
    DrawShape/SceneSCone.cpp \
    SceneShape.cpp
