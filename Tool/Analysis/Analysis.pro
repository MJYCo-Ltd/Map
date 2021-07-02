CONFIG -= qt
TARGET = Analysis
TEMPLATE = lib

SDK_CONFIG *= TOOL
include($$PWD/../../SoftSDK.pri)

HEADERS += \
    AnalysisTool.h \
    ViewAnalysis.h

SOURCES += \
    AnalysisTool.cpp \
    ViewAnalysis.cpp
