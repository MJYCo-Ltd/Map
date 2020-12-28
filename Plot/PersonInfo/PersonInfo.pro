#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:02:02
#
#-------------------------------------------------

QT       -= gui

TARGET = PersonInfo
TEMPLATE = lib

DEFINES += PERSONINFO_LIBRARY

SDK_CONFIG *= OSG OsgExtern SceneCore Satellite

include($$PWD/../../SoftSDK.pri)

HEADERS += \
    MineInfo.h \
    PersonInfo.h

SOURCES += \
    MineInfo.cpp \
    PersonInfo.cpp
