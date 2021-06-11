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

lessThan(QT_MAJOR_VERSION, 6):CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 5):CONFIG += c++17

INCLUDEPATH *= $$PWD/../Inc $$PWD/../Inc/Plot
win32{
    DEFINES -= UNICODE
    # 开启utf-8 编码方式支持
    QMAKE_CXXFLAGS += /utf-8
    QMAKE_CXXFLAGS += /wd"4100"
    QMAKE_CXXFLAGS += /MP

    LIBS *= -L$$PWD/../Lib
    DLLDESTDIR = $$PWD/../../Bin/MapPlugin/Plot
}

unix{
    DESTDIR = $$PWD/../../Bin/MapPlugin/Plot
    LIBS *= -L$$PWD/../../Bin
    contains(TEMPLATE, "app"){
        QMAKE_LFLAGS += -Wl,-rpath=.:./osglib:./stklib
    }else{
        QMAKE_CXXFLAGS += -fvisibility=hidden
    }
}

NEWGL3PATH = D:/MyData/Tencent/QYWX/WXWork/1688851982861420/WeDrive/石家庄沐吉源科技有限公司/石家庄沐吉源科技有限公司/研发部/NewGL3

contains(SDK_CONFIG,OSG){

# include file
    win32{
        INCLUDEPATH *=$${NEWGL3PATH}/include
        LIBS *= -L$${NEWGL3PATH}/lib
    }

    unix{
        LIBS *= -L$$PWD/../../Bin/osglib
    }

   CONFIG (debug, debug|release){
#threads
       LIBS *= -lOpenThreadsd
# osg libs
       LIBS *= -losgd -losgDBd -losgGAd -losgSimd -losgViewerd -losgUtild -losgTextd -losgManipulatord -losgWidgetd
#osgEarth libs
       LIBS *= -losgEarthd
   }else{
       LIBS *= -lOpenThreads
       LIBS *= -losg -losgDB -losgGA -losgSim -losgViewer -losgUtil  -losgText  -losgManipulator  -losgWidget
       LIBS *= -losgEarth
   }
}

contains(SDK_CONFIG,Satellite){
    MathPath=$${PWD}/../../VersionMath
    INCLUDEPATH *= $${MathPath}/Inc $${MathPath}/Inc/Math
    win32:LIBS *= -L$${MathPath}/Lib
    unix:LIBS *= -L$$PWD/../../Bin/stklib
    CONFIG (debug, debug|release){
        LIBS *= -lSatellited -lMathd -lGisMathd -lSatelliteToolKitd
    }else{
        LIBS *= -lSatellite -lMath -lGisMath -lSatelliteToolKit
    }
}

contains(SDK_CONFIG,OsgExtern){
    CONFIG (debug, debug|release){
        LIBS *= -lOsgExternd
    }else{
        LIBS *= -lOsgExtern
    }
}

contains(SDK_CONFIG,SceneCore){
    CONFIG (debug, debug|release){
        LIBS *= -lSceneCored
    }else{
        LIBS *= -lSceneCore
    }
}

# This can suffix a d by itself,if is a debug version
CONFIG(debug, debug|release) {
    OBJECTS_DIR = $$PWD/../debug/$$TARGET/obj
    MOC_DIR = $$PWD/../debug/$$TARGET/moc
    UI_DIR = $$PWD/../debug/$$TARGET/UI
    TARGET = $$join(TARGET,,,d)
}else{
    OBJECTS_DIR = $$PWD/../release/$$TARGET/obj
    MOC_DIR = $$PWD/../release/$$TARGET/moc
    UI_DIR = $$PWD/../release/$$TARGET/UI
}
