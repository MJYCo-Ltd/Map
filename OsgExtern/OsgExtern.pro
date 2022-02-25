CONFIG -= qt

TARGET = OsgExtern
TEMPLATE = lib

DEFINES += OSGEXTERN_LIBRARY

SDK_CONFIG *= OSG Satellite

include(../SoftSDK.pri)

SOURCES += \
    FlashAbility.cpp \
    FlatAbility.cpp \
    Grid2TriangleStrip.cpp \
    JudgeGeometry.cpp \
    LoadGLSL.cpp \
    MathCommon.cpp \
    MyMatrixTransform.cpp \
    MyPositionAttitudeTransform.cpp \
    NodeAbilityManager.cpp \
    SplitRectangle.cpp

HEADERS += \
    FlashAbility.h \
    FlatAbility.h \
    ImplNodeAbility.hpp
