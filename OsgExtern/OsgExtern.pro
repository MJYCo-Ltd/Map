CONFIG -= qt

TARGET = OsgExtern
TEMPLATE = lib

DEFINES += OSGEXTERN_LIBRARY

SDK_CONFIG *= OSG Satellite

include(../SoftSDK.pri)

SOURCES += \
    Grid2TriangleStrip.cpp \
    MathCommon.cpp \
    MyMatrixTransform.cpp \
    MyPositionAttitudeTransform.cpp \
    SplitRectangle.cpp
