CONFIG -= qt
TARGET = Analysis
TEMPLATE = lib

include($$PWD/../Tool.pri)

HEADERS += \
    AnalysisTool.h \
    ViewAnalysis.h

SOURCES += \
    AnalysisTool.cpp \
    ViewAnalysis.cpp
