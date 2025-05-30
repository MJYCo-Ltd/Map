#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

CONFIG -= qt

TARGET = Hud
TEMPLATE = lib

SDK_CONFIG *= OSG OsgExtern PLOT
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    HudImage.h \
    HudLayout.h \
    HudShape.h \
    HudText.h \
    HudButton.h

SOURCES += \
    HudImage.cpp \
    HudLayout.cpp \
    HudShape.cpp \
    HudText.cpp \
    HudButton.cpp
