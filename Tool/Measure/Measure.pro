CONFIG -= qt
TARGET = Measure
TEMPLATE = lib

SDK_CONFIG *= TOOL
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    MeasureTool.h

SOURCES += \
    MeasureTool.cpp
