#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT -= gui

TARGET = SpaceBackGround
TEMPLATE = lib

DEFINES += SPACEENV_LIBRARY

SDK_CONFIG *= OSG Satellite OsgExtern SceneCore PLOT

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    SkyNode.h \
    Solar/PlanetModel.h \
    Solar/SolarEnv.h \
    Solar/SunModel.h \
    SpaceBackGround.h \
    Star/Boundary.h \
    Star/Frustum.h \
    Star/GeodesicGrid.h \
    Star/Milkyway.h \
    Star/Star.h \
    Star/StarConstellation.h \
    Star/StarEnv.h \
    Star/StarManager.h \
    Star/StarRender.h \
    Star/StarToneReproducer.h \
    Star/StarZoneArray.h \
    Star/StarZoneArrayBase.h \
    Star/StarZoneArrayRenderer.h \
    Star/StarZoneData.h \
    Star/StarZoneDataBase.h

SOURCES += \
    SkyNode.cpp \
    Solar/PlanetModel.cpp \
    Solar/SolarEnv.cpp \
    Solar/SunModel.cpp \
    SpaceBackGround.cpp \
    Star/Boundary.cpp \
    Star/Frustum.cpp \
    Star/GeodesicGrid.cpp \
    Star/Milkyway.cpp \
    Star/Star.cpp \
    Star/StarConstellation.cpp \
    Star/StarEnv.cpp \
    Star/StarManager.cpp \
    Star/StarRender.cpp \
    Star/StarToneReproducer.cpp \
    Star/StarZoneArrayRenderer.cpp

