#include <iostream>
#include <iomanip>
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include <QThread>

#include <osgEarth/GeoData>
#include <osg/Matrix>

#include <ISceneCore.h>
#include <SceneGraph/IWindow.h>
#include <Plot/Map/SpaceEnv/ISpaceEnv.h>
#include <Plot/Map/IMap.h>
#include <VersionMathCommon.h>
#include <Math/VecMat.h>
#include <Math/YPRAngle.h>
#include <GisMath/GisMath.h>
#include <Satellite/Date.h>
#include <Satellite/SGP4.h>
#include <Satellite/CoorSys.h>
#include <SatelliteToolKit/SatelliteToolKit.h>


#include "mainwindow.h"

/**
 * @brief 窗口消息定义
 */
class WindowMessage:public IWindowMessageObserver
{
public:
    void KeyUp(char chr){qDebug()<<(int)chr;}
    void MouseDown(MouseButtonMask type, int, int)
    {
        switch (type)
        {
        case LEFT_MOUSE_BUTTON:
            qDebug()<<"Left Mouse";
            break;
        case RIGHT_MOUSE_BUTTON:
            qDebug()<<"right Mouse";
            break;
        case MIDDLE_MOUSE_BUTTON:
            qDebug()<<"Middle Mouse";
            break;
        }
        qDebug()<<"Mouse down";
    }
};

int main(int argc, char *argv[])
{
    bool bChecked = CheckPC(argv);
#ifdef NEED_VR
    bChecked = CheckVR();
#endif

    if(!bChecked)
    {
        return (-1);
    }

    QApplication a(argc, argv);

    MainWindow w;
    w.show();



    auto pScene = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(ISceneGraph::SCENEGRAPH_2D);
    //pScene->GetMap()->GetSpaceEnv()->SetMilkywayVisible(false);
//    for(auto one:pScene->GetMap()->GetMapLayers())
//    {
//        qDebug()<<*one.c_str();
//    }

    w.SetSecenGraph(pScene);

    //pScene->GetMainWindow()->SubMessage(new WindowMessage);

    QWidget* pWindow = pScene->GetMainWindow()->ToWidget();
//    auto pNewWindow = pScene->CreateNewWindow();
//    auto pOtherWidget = pNewWindow->ToWidget();
//    pOtherWidget->show();
    if(nullptr != pWindow)
    {
        w.setCentralWidget(pWindow);
    }
//    qDebug()<<QThread::currentThread();

    pScene->Run();


    return a.exec();
}
