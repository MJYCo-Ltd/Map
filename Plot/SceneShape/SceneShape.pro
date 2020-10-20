#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = SceneShape
TEMPLATE = lib

SDK_CONFIG *= OSG Satellite OsgExtern
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    DrawShape/SceneCone.h \
    DrawShape/SceneLine.h \
    DrawShape/ScenePoint.h \
    DrawShape/ScenePulse.h \
    DrawShape/SceneSCone.h \
    SceneShape.h

SOURCES += \
    DrawShape/SceneCone.cpp \
    DrawShape/SceneLine.cpp \
    DrawShape/ScenePoint.cpp \
    DrawShape/ScenePulse.cpp \
    DrawShape/SceneSCone.cpp \
    SceneShape.cpp
