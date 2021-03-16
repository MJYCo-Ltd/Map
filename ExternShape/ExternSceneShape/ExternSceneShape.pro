#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = ExternMapShape
TEMPLATE = lib

SDK_CONFIG *= OSG Satellite OsgExtern
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    SceneShape.h

SOURCES += \
    SceneShape.cpp
