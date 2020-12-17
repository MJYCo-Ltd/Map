#-------------------------------------------------
#
# Project created by QtCreator 2019-10-19T11:35:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoadMap
TEMPLATE = app


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

SDK_CONFIG *= SceneCore Satellite OSG
unix:SDK_CONFIG *= Satellite OsgExtern
include(../SoftSDK.pri)
