#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = SatelliteShow
TEMPLATE = lib

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite PLOT

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    SatelliteShow.h

SOURCES += \
    SatelliteShow.cpp
