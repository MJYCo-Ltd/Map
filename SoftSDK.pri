# Attention Debug   version always suffix with a d,
#           Release version always not.
# For example
# CONFIG(debug, debug|release){
#         LIBS += -losgd
#}else{
#         LIBS += -losg
#}
#
#Attention DLLDESTDIR only useful in windows
#          This will only copy exe or dll to DLLDESTDIR

SDKPath = G:\new\LXQ\VS2017

# 开启utf-8 编码方式支持
win32-msvc:QMAKE_CXXFLAGS += -utf-8
win32-msvc:QMAKE_CXXFLAGS += /wd"4100"

unix{
    QMAKE_CXXFLAGS += -fvisibility=hidden
}
# This can change by Qmake, We don't need change by yourself.
LIBPDIR = $${SDKPath}/lib/x64/OSGEARTH/lib

# Set the path for find libs
LIBS *= -L$${LIBPDIR}

contains(SDK_CONFIG,OSG){

# include file
    INCLUDEPATH *=$${SDKPath}/include/OSGEARTH

   CONFIG (debug, debug|release){
#threads
       LIBS *= -lOpenThreadsd
# osg libs
       LIBS *= -losgd -losgDBd -losgGAd -losgSimd -losgViewerd -losgUtild -losgTextd -losgManipulatord -losgWidgetd
#osgEarth libs
       LIBS *= -losgEarthd -losgEarthUtild
   }else{
       LIBS *= -lOpenThreads
       LIBS *= -losg -losgDB -losgGA -losgSim -losgViewer -losgUtil  -losgText  -losgManipulator  -losgWidget
       LIBS *= -losgEarth -losgEarthUtil
   }
}

contains(SDK_CONFIG,Satellite){
    CONFIG (debug, debug|release){
        LIBS *= -lSatellited -lMathd
    }else{
        LIBS *= -lSatellite -lMath
    }
}

contains(SDK_CONFIG,OsgExtern){
    CONFIG (debug, debug|release){
        LIBS *= -lOsgExternd
    }else{
        LIBS *= -lOsgExtern
    }
}
# This can suffix a d by itself,if is a debug version
CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
}
