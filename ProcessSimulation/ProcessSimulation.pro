QT += core qml sql

TEMPLATE = lib
DEFINES += PROCESSSIMULATION_LIBRARY

include(../SoftSDK.pri)

SOURCES += \
    CommandBuildComponent.cpp \
    ProcessBuild.cpp \
    Simulation.cpp
	
HEADERS += \
    CommandBuildComponent.h \
    ProcessBuild.h \
    ProcessSimulation_global.h \
    Simulation.h \     
    Process.h
