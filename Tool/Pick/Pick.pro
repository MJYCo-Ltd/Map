QT -= gui
TARGET = Pick
TEMPLATE = lib

SDK_CONFIG *= OSG TOOL
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    IntersectionPick.h \
    PickTool.h \
    PointPick.h

SOURCES += \
    IntersectionPick.cpp \
    PickTool.cpp \
    PointPick.cpp
