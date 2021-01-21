QT -= gui
TARGET = Pick
TEMPLATE = lib

SDK_CONFIG *= OSG
include($$PWD/../Tool.pri)

HEADERS += \
    PickTool.h \
    PointPick.h

SOURCES += \
    PickTool.cpp \
    PointPick.cpp
