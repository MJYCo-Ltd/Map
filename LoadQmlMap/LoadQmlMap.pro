QT += quick

HEADERS += \
    AppGlobal.h \
    DealWindowsMessage.h \
    PlotMap.h

SOURCES += \
        AppGlobal.cpp \
        DealWindowsMessage.cpp \
        PlotMap.cpp \
        main.cpp

lessThan(QT_MAJOR_VERSION, 6):RESOURCES += qml.qrc
greaterThan(QT_MAJOR_VERSION, 5):RESOURCES += qml6.qrc


TRANSLATIONS += \
    LoadQmlMap_zh_CN.ts

SDK_CONFIG *= SceneCore
unix:SDK_CONFIG *= Satellite OsgExtern
include(../SoftSDK.pri)
