QT += core qml sql

TEMPLATE = lib
DEFINES += PROCESSSIMULATION_LIBRARY

include(../SoftSDK.pri)

SOURCES += \
    Simulation.cpp \
    Process.cpp \
    ProcessConstruction.cpp \
    ProcessConstructionJson.cpp 
	
HEADERS += \
    ProcessSimulation_global.h \
    Simulation.h \     
    Process.h \
    ProcessConstruction.h \
    ProcessConstructionJson.h
