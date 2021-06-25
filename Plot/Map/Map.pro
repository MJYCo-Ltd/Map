CONFIG -= qt
TARGET = Map
TEMPLATE = lib

DEFINES += MAP_LIBRARY

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite PLOT

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    Atmosphere.h \
    Map.h \
    MapLayer.h \
    MapModelLayer.h \
    MapModifyLayer.h \
    MapNodeChanged.h \
    SpaceEnv.h

SOURCES += \
    Atmosphere.cpp \
    Map.cpp \
    MapLayer.cpp \
    MapModelLayer.cpp \
    MapModifyLayer.cpp \
    MapNodeChanged.cpp \
    SpaceEnv.cpp
