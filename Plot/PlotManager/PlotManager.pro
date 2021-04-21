QT -= gui
TARGET = Plot
TEMPLATE = lib

SDK_CONFIG *= OSG SceneCore OsgExtern Satellite
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    MapNodeFactory.h \
    PlotManager.h

SOURCES += \
    MapNodeFactory.cpp \
    PlotManager.cpp
