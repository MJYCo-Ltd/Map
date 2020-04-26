#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui

TARGET = SpaceEnv
TEMPLATE = lib

DEFINES += SPACEENV_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SDK_CONFIG *= OSG Satellite OsgExtern

include(../SoftSDK_My.pri)

INCLUDEPATH *= $$PWD/../Inc

DESTDIR = $$PWD/../Bin/new
DLLDESTDIR = F:\TestGL3\bin

LIBS *= -L$$DESTDIR
CONFIG (debug, debug|release){
    LIBS *= -lSceneCored
}else{
    LIBS *= -lSceneCore
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    Solar/PlanetModel.h \
    Solar/SolarEnv.h \
    Solar/SunModel.h \
    SpaceEnv.h \
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
    Solar/PlanetModel.cpp \
    Solar/SolarEnv.cpp \
    Solar/SunModel.cpp \
    SpaceEnv.cpp \
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

