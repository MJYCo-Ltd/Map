#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui

TARGET = ExternMapShape
TEMPLATE = lib

DEFINES *= EXTERNMAPSHAPE_LIBRARY

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    ExternMapShape_Global.h \
    MapRectange.h

SOURCES += \
    MapRectange.cpp
