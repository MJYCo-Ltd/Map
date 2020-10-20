#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui

TARGET = MapShape
TEMPLATE = lib

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    DrawShape/MapCoverImage.h \
    DrawShape/MapLine.h \
    DrawShape/MapLocation.h \
    DrawShape/MapPoint.h \
    DrawShape/MapPolygon.h \
    Grid2TriangleStrip.h \
    MapShape.h

SOURCES += \
    DrawShape/MapCoverImage.cpp \
    DrawShape/MapLine.cpp \
    DrawShape/MapLocation.cpp \
    DrawShape/MapPoint.cpp \
    DrawShape/MapPolygon.cpp \
    Grid2TriangleStrip.cpp \
    MapShape.cpp
