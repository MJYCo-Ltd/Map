CONFIG -= qt
TARGET = Sound
TEMPLATE = lib

SDK_CONFIG *= TOOL
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    SoundTool.h

SOURCES += \
    SoundTool.cpp
