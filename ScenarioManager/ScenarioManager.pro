QT += core qml

TEMPLATE = lib
DEFINES += SCENARIOMANAGER_LIBRARY

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
