QT += core qml

TEMPLATE = lib
DEFINES += SCENARIOMANAGER_LIBRARY

SDK_CONFIG *= Satellite OSG
include(../SoftSDK.pri)

SOURCES += \
    Scenario.cpp \
    ScenarioItem.cpp \
    ScenarioManager.cpp

HEADERS += \
    Scenario.h \
    ScenarioItem.h \
    ScenarioManager_global.h \
    ScenarioManager.h
