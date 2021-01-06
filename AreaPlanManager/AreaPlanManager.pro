QT += core qml

TEMPLATE = lib
DEFINES += AREAPLANMANAGER_LIBRARY

include(../SoftSDK.pri)

SOURCES += \
    AreaPlan.cpp \
    AreaPlanLayer.cpp \
    AreaPlanManager.cpp \
    AreaPolygon.cpp \
    AreaPolygonEditor.cpp \
    AreaPolygonLoader.cpp

HEADERS += \
    AreaPlan.h \
    AreaPlanLayer.h \
    AreaPlanManager_global.h \
    AreaPlanManager.h \
    AreaPolygon.h \
    AreaPolygonEditor.h \
    AreaPolygonLoader.h
