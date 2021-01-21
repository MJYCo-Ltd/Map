QT -= gui
TARGET = Tool
TEMPLATE = lib

SDK_CONFIG *= SceneCore OSG
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    ToolSelector.h

SOURCES += \
    ToolSelector.cpp
