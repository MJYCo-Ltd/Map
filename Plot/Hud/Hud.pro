#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = Hud
TEMPLATE = lib

SDK_CONFIG *= OSG OsgExtern
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    HudImage.h \
    HudShape.h \
    HudText.h

SOURCES += \
    HudImage.cpp \
    HudShape.cpp \
    HudText.cpp
