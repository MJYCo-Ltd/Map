#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:35:17
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoadMap
TEMPLATE = app

DEFINES += NEED_VR
SOURCES += \
        CSimpleLabel.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        CSimpleLabel.h \
        mainwindow.h

FORMS += \
        mainwindow.ui \
        mainwindow.ui

SDK_CONFIG *= SceneCore Satellite OSG ExternShape
unix:SDK_CONFIG *= Satellite OsgExtern ExternShape

include(../SoftSDK.pri)
