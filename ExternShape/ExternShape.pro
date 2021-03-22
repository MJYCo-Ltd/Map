TARGET = ExternShape
TEMPLATE = lib

DEFINES *= EXTERNMAPSHAPE_LIBRARY

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include($$PWD/../SoftSDK.pri)

HEADERS +=

SOURCES += \
    MapPlaceNode.cpp \
    MapRectange.cpp
CONFIG *= ordered
