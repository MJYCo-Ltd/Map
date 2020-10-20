QT -= gui
TARGET = Map
TEMPLATE = lib

DEFINES += MAP_LIBRARY

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include(../SoftSDK.pri)

HEADERS += \
    Map.h \
    MapLayer.h \
    MapModelLayer.h \
    MapModifyLayer.h \
    MapNodeChanged.h \
    Map_Global.h \
    SpaceEnv.h

SOURCES += \
    Map.cpp \
    MapLayer.cpp \
    MapModelLayer.cpp \
    MapModifyLayer.cpp \
    MapNodeChanged.cpp \
    SpaceEnv.cpp
