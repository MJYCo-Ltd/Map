QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH *= $$PWD/../Inc

DESTDIR = $$PWD/../../Bin


HEADERS += \
    AppGlobal.h \
    PlotMap.h

SOURCES += \
        AppGlobal.cpp \
        PlotMap.cpp \
        main.cpp

RESOURCES += qml.qrc \
    img.qrc

TRANSLATIONS += \
    LoadQmlMap_zh_CN.ts

SDK_CONFIG *= SceneCore
unix:SDK_CONFIG *= Satellite OsgExtern
include(../SoftSDK.pri)
LIBS *= -lScenarioManagerd

DISTFILES +=
