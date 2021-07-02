QT -= gui
TARGET = Pick
TEMPLATE = lib

SDK_CONFIG *= OSG TOOL
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    PickTool.h \
    PointPick.h

SOURCES += \
    PickTool.cpp \
    PointPick.cpp
