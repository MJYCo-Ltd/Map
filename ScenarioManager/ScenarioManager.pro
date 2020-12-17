QT += core qml widgets

TEMPLATE = lib
DEFINES += SCENARIOMANAGER_LIBRARY

include(../SoftSDK.pri)

SOURCES += \
    ScenarioItem.cpp \
    ScenarioManager.cpp

HEADERS += \
    ScenarioItem.h \
    ScenarioManager_global.h \
    ScenarioManager.h
