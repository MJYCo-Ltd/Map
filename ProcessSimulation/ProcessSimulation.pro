QT += core qml sql widgets

TEMPLATE = lib
DEFINES += PROCESSSIMULATION_LIBRARY

include(../SoftSDK.pri)

SOURCES += \
    CommandBuildComponent.cpp \
    ProcessBuild.cpp \
    ProcessBuildData.cpp \
    ProcessSimulationBuild.cpp \
    Simulation.cpp
	
HEADERS += \
    CommandBuildComponent.h \
    ProcessBuild.h \
    ProcessBuildData.h \
    ProcessSimulationBuild.h \
    ProcessSimulation_global.h \
    Simulation.h \     
    Process.h
