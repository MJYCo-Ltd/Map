#include <iostream>
#include <iomanip>
#include "mainwindow.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include <ISceneCore.h>
#include <SceneGraph/IWindow.h>
#include <SpaceEnv/ISpaceEnv.h>
#include <Map/IMap.h>
#include <Sofa/sofa.h>
#include <Math/VecMat.h>
#include <Satellite/Date.h>
#include <Satellite/SGP4.h>
#include <Satellite/CoorSys.h>
#include <Math/YPRAngle.h>
#include <GisMath/GisMath.h>
#include <SatelliteToolKit/SatelliteToolKit.h>
#include <osgEarth/GeoData>
#include <osg/Matrix>


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
    QApplication a(argc, argv);

    bool bChecked = CheckPC();
    std::string sErrorInfo;
    InitSatelliteToolKit("E:/Git/Bin",sErrorInfo);

    std::cout<<sErrorInfo<<std::endl;

    if(!bChecked)
    {
        return(1);
    }
#ifdef Q_OS_WIN
    char * pStr = strrchr(argv[0],'\\');
#else
    char * pStr = strrchr(argv[0],'/');
#endif
    pStr[0] = 0;
    SetExePath(argv[0]);

    CDate mjBein(2020,3,30,2,35,38.254,UTC);
    Satellite::CSGP4 spg41("1 91001U          20061.66666667 -.00000001  00000-0 -13106-2 0 00008",
                          "2 91001 045.0073 000.0048 0004655 268.5152 091.4846 07.15404217000017");

    double dMJD = mjBein.GetMJD();

    Math::CVector vPV = spg41.CalPV(dMJD);

    CVector vECI;
    CCoorSys::TEME2ECI(dMJD,vPV,vECI);
    Math::CVector vECF;
    CCoorSys::TEME2ECF(dMJD,vPV,vECF);

    CVector vOther;
    GisMath::XYZ2LBH(vECF.slice(0,2),vOther);

    PV satPV;
    satPV.stP.dX = vECF(0);
    satPV.stP.dY = vECF(1);
    satPV.stP.dZ = vECF(2);

    satPV.stV.dX = vECF(3);
    satPV.stV.dY = vECF(4);
    satPV.stV.dZ = vECF(5);

    vOther(1) += 10*DD2R;

    Pos satPRY,rPos;

    GisMath::LBH2XYZ(vOther(0),vOther(1),vOther(2),rPos.dX,rPos.dY,rPos.dZ);

    CVector vEarthPoint(rPos.dX,rPos.dY,rPos.dZ);
    std::cout<<QString::fromUtf8("原始位置").toLocal8Bit().data()<<std::endl;
    std::cout<<setw(20)<<setprecision(12)<<vEarthPoint<<std::endl;

    double dLength = (vEarthPoint-vECF.slice(0,2)).Length();


    CalPRY(satPV,rPos,Rota_RYP,satPRY);

    /// 计算全局坐标系到卫星本体坐标系的旋转矩阵
    CVector Pos(satPV.stP.dX,satPV.stP.dY,satPV.stP.dZ);
    CVector rZ = -Pos;
    CVector V(satPV.stV.dX,satPV.stV.dY,satPV.stV.dZ);

    CVector rY = CVecMat::Cross(rZ , V);
    CVector rX = CVecMat::Cross(rY,rZ);

    CMatrix J2000ECF = CCoorSys::ECI2ECF(dMJD);

    CVector vJ200 = J2000ECF*vECI.slice(0,2);


    std::cout<<setw(20)<<setprecision(12)<<(vJ200 - vECF.slice(0,2))<<std::endl;
    std::cout<<setw(20)<<setprecision(12)<<(vJ200 - vECF.slice(0,2)).Length()<<std::endl;

    rX.Normalize();
    rY.Normalize();
    rZ.Normalize();

    CMatrix RoateMatrix(3,3);
    RoateMatrix.SetRow(0,rX);
    RoateMatrix.SetRow(1,rY);
    RoateMatrix.SetRow(2,rZ);
    osg::Matrix matOsg;

    matOsg.set(RoateMatrix(0,0),RoateMatrix(0,1),RoateMatrix(0,2),0.,
               RoateMatrix(1,0),RoateMatrix(1,1),RoateMatrix(1,2),0.,
               RoateMatrix(2,0),RoateMatrix(2,1),RoateMatrix(2,2),0.,
               0.,0.,0.,1.);

    matOsg.postMultTranslate(osg::Vec3f(vJ200(0),vJ200(1),vJ200(2)));

    CMatrix rotateMatrix = CYPRAngle::CreateMatrix(satPRY.dX,satPRY.dY,satPRY.dZ,RYP);
    matOsg.preMult(osg::Matrix(rotateMatrix(0,0),rotateMatrix(0,1),rotateMatrix(0,2),0.,
                               rotateMatrix(1,0),rotateMatrix(1,1),rotateMatrix(1,2),0.,
                               rotateMatrix(2,0),rotateMatrix(2,1),rotateMatrix(2,2),0.,
                               0.,0.,0.,1.));
    osg::Vec3f vOsgPos = matOsg.preMult(osg::Vec3f(0,0,dLength));

    CVector osgCalPos(vOsgPos.x(),vOsgPos.y(),vOsgPos.z());

    std::cout<<QString::fromUtf8("OSG计算出来的位置").toLocal8Bit().data()<<std::endl;
    std::cout<<setw(20)<<setprecision(12)<<osgCalPos<<std::endl;

    std::cout<<QString::fromUtf8("两者差值").toLocal8Bit().data()<<std::endl;
    std::cout<<setw(20)<<setprecision(12)<<(osgCalPos-vEarthPoint)<<std::endl;

    std::cout<<QString::fromUtf8("目标距离星的距离(m)").toLocal8Bit().data()<<std::endl;
    std::cout<<dLength<<std::endl;

    std::cout<<QString::fromUtf8("原始位置与OSG计算出来值的差别(m)").toLocal8Bit().data()<<std::endl;
    std::cout<<(osgCalPos-vEarthPoint).Length()<<std::endl;

    osgEarth::GeoPoint point(osgEarth::SpatialReference::get("wgs84"),vOther(0)*DR2D,vOther(1)*DR2D,vOther(2),osgEarth::ALTMODE_ABSOLUTE);
    osg::Vec3d vEarth3D;
    point.toWorld(vEarth3D);

    CVector vEarthLocal(vEarth3D.x(),vEarth3D.y(),vEarth3D.z());
    std::cout<<QString::fromUtf8("OSGEarth 标绘的位置").toLocal8Bit().data()<<std::endl;
    std::cout<<setw(20)<<setprecision(12)<<vEarthLocal<<std::endl;
    std::cout<<QString::fromUtf8("原始位置与OSGEarth计算出来值的差别(m)").toLocal8Bit().data()<<std::endl;
    std::cout<<(vEarthLocal-vEarthPoint).Length()<<std::endl;
    MainWindow w;
    w.show();



    auto pScene = GetSceneCore()->GetSceneGraphManager()->CreateSceneGraph(SCENEGRAPH_3D);
    //pScene->GetMap()->GetSpaceEnv()->SetMilkywayVisible(false);
//    for(auto one:pScene->GetMap()->GetMapLayers())
//    {
//        qDebug()<<*one.c_str();
//    }

    w.SetSecenGraph(pScene);

    //pScene->GetMainWindow()->SubMessage(new WindowMessage);

    QWidget* pWindow = pScene->GetMainWindow()->ToWidget();
    if(nullptr != pWindow)
    {
        w.setCentralWidget(pWindow);
    }

    pScene->Run();


    return a.exec();
}
