﻿#include <QFileDialog>
#include <QDebug>

#include <VersionMathCommon.h>
#include <GisMath/GisMath.h>
#include <Satellite/Date.h>
#include <Satellite/SGP4.h>
#include <Satellite/CoorSys.h>
#include <SatelliteToolKit/SatelliteToolKit.h>

#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <Plot/Map/IMap.h>
#include <Plot/Map/IMapLayer.h>
#include <Plot/Map/SpaceEnv/ISpaceEnv.h>
#include <Plot/Map/SpaceEnv/ISpaceBackGround.h>
#include <Plot/IPlot.h>
#include <Plot/Map/IMapLine.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/Map/IMapCoverImage.h>
#include <Plot/Common/ISceneModel.h>
#include <Plot/Map/IMapLocation.h>
#include <Plot/SceneShape/IPoint.h>
#include <Plot/SceneShape/IPolygon.h>
#include <Plot/SceneShape/IConeSensor.h>
#include <Plot/SceneShape/ISConeSensor.h>
#include <Plot/Common/ISceneFlashGroup.h>
#include <Plot/SceneShape/IEllipsoidSensor.h>
#include <Plot/SatelliteShow/ISatellite.h>
#include <Plot/Hud/IHudText.h>
#include <Plot/Hud/IHudImage.h>
#include <Plot/Hud/IHudLayout.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/Common/ISceneScaleGroup.h>
#include <Plot/Common/ISceneLodGroup.h>
#include <Plot/SceneShape/ILabel.h>
#include <Plot/Common/ISceneScreenGroup.h>
#include <Plot/Common/ISceneAttitudeGroup.h>
#include <Plot/SceneShape/IBoxSensor.h>
#include <Plot/Map/IMapPolygon3D.h>
#include <Hud/IViewHud.h>
#include <Inner/ILoadResource.h>
#include <Plot/Map/IMapObserver.h>
#include <Plot/SceneShape/IPolygon3D.h>
#include <Ability/IFlashAbility.h>
#include <ExternShape/MapPlaceNode.h>
#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapPoint.h>
#include <Plot/Map/IMapPolygon.h>
#include <Plot/Common/ISceneVisualGroup.h>
#include <Ability/IFlatAbility.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ExternShape/MapRectange.h>
#include <ExternShape/MapCircle.h>

struct MapObser:public IMapMessageObserver,public IWindowMessageObserver
{
    MapObser(ISceneGraph* pSceneGraph):m_pSceneGraph(pSceneGraph){}

    void MousePos(double dLon,double dLat,double dHeight)
    {
        pos.dX = dLon;
        pos.dY = dLat;
        pos.dZ = dHeight;
//        std::cout<<dLon<<','<<dLat<<','<<dHeight<<std::endl;
    }
//    void MouseDown(MouseButtonMask, int, int)
//    {
//        if(nullptr == pLayer)
//        {
//            pLayer = m_pSceneGraph->GetMap()->CreateLayer("HHHH");
//            auto pMapPolygon=dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));

//            pLayer->AddSceneNode(pMapPolygon);
//            m_pPolygon = pMapPolygon->GetDrawPolygon();
//        }

//        m_pPolygon->AddPoint(m_pPolygon->GetCount()-1,pos);
//    }
private:
    ScenePos pos;
    IMapLayer* pLayer=nullptr;
    ISceneGraph* m_pSceneGraph;
    IPolygon*    m_pPolygon;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    startTimer(100);
    ui->setupUi(this);
    ui->mainToolBar->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSecenGraph(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
    if(m_pSceneGraph->GetMap())
    {
//        m_pSceneGraph->GetMap()->LoadUserMap("E:/Git/Bin/Data/Earth/Geocentric.earth",false);
        auto pMap= new MapObser(m_pSceneGraph);
        m_pSceneGraph->GetMainWindow()->SubMessage(pMap);
        m_pSceneGraph->GetMap()->SubMessage(pMap);
        m_pSceneGraph->GetMap()->SetEarthSelfRotate(false);
        m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->HomeViewPoint();
        //m_pSceneGraph->GetMap()->GetSpaceEnv()->SetVisible(false);
//        m_pSceneGraph->GetMap()->GetSpaceEnv()->ShowSpaceBackGround(false);
//        m_pSceneGraph->GetMap()->OpenLight(false);
//        m_pSceneGraph->GetMap()->SetShowAtmosphere(false);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    static int nIndex{};
    if(nullptr != m_pTestNode)
    {
        QString sImagePath=QString("Image/Mine/%1.png").arg((nIndex++%74) + 1);
        m_pTestNode->SetImagePath(sImagePath.toStdString());
        m_pTestNode->SetText(sImagePath.toStdString());
        SceneColor color;
        color.fG = color.fB = 0.f;
        color.fR = float(nIndex++%74)/74;
        m_pTestNode1->SetColor(color);
    }
}

bool bClick=false;
void MainWindow::on_actionchange_triggered()
{
    if(!bClick)
    {
        m_pSceneGraph->GetMap()->ChangeMapType(MAP_2D);
        bClick = true;
    }
    else
    {
        m_pSceneGraph->GetMap()->ChangeMapType(MAP_3D);
        bClick = false;
    }
}

bool bShowBackGround=false;
IHudLayout* pHudLayout=nullptr;
IHudText*   pHudText=nullptr;
IMapLocation* pEarthLocation=nullptr;
ISceneFlashGroup* pFlash=nullptr;
ISatellite*  pSatellite = nullptr;

void MainWindow::on_action_triggered()
{
    int nX(width()/2),nY(height()/2);
    ScenePos pos;
    qDebug()<<nX<<','<<nY;
    m_pSceneGraph->GetMap()->ConvertCoord(nX,nY,pos,0);
    m_pSceneGraph->GetMap()->ConvertCoord(nX,nY,pos,1);
    qDebug()<<nX<<','<<nY;
    pos.dX += 1.;

    m_pLayer = m_pSceneGraph->GetMap()->CreateLayer("Test");
    TestGroup();
    TestHud();
    m_pSceneGraph->GetMap()->OpenLight(true);
    ScenePos pos11;
    pos11.dX = 126.0;
    pos11.dY = 45.6;
    pos11.dZ = 10;

    SceneColor color11;
    color11.fR =1.0;
    color11.fG =0.0;
    color11.fB =0.0;

//    CMapCircle* pCircle = new CMapCircle(m_pSceneGraph);
//    pCircle->SetCenter(pos11);
//    pCircle->SetRadius(10000);
//    pCircle->SetColor(color11);

//    ISceneGroup* pSceneRootaaaa = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);
//    IMapLocation*  aaaa = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
//    aaaa->SetSceneNode(pSceneRootaaaa);
//    pSceneRootaaaa->AddSceneNode(pCircle->GetMapSceneNode());
//    IMapLayer* m_pLayerTest = m_pSceneGraph->GetMap()->CreateLayer("test");
//    m_pLayerTest->AddSceneNode(aaaa);

//    CMapRectange* pRectangle = new CMapRectange(m_pSceneGraph);

//    ISceneGroup* pSceneRootaaaa = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);
//    IMapLocation*  aaaa = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));

//    aaaa->SetSceneNode(pSceneRootaaaa);
//    pSceneRootaaaa->AddSceneNode(pRectangle->GetMapSceneNode());
//    IMapLayer* m_pLayerTest = m_pSceneGraph->GetMap()->CreateLayer("test");
//    m_pLayerTest->AddSceneNode(aaaa);

//    MapGeoPos pos11,pos21;
//    pos11.fX = -126.0;
//    pos11.fY = 45.6;
//    pos11.fZ = 10000;


//    pos21.fX = -136.0;
//    pos21.fY = 55.6;
//    pos21.fZ = 10000;
//    pRectangle->SetPos1(pos11);
//    pRectangle->SetPos2(pos21);

//    SceneColor color11;
//    color11.fR =1.0;
//    color11.fG =0.0;
//    color11.fB =0.0;
//    pRectangle->SetColor(color11);


    SceneColor color;

    if(MAP_3D == m_pSceneGraph->GetMap()->GetMapType())
    {
        /// 标绘卫星
        pSatellite= dynamic_cast<ISatellite*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISatellite"));

        pSatellite->SetName("卫星");
        pSatellite->SetScale(10000.);
        pSatellite->SetPicPath("Image/ship.png");
//        IFlashAbility* pFLash = pSatellite->GetOrCreateAbility(FLASH_ABILITY)->AsFlashAbility();
//        color.fB = 1.0f;
//        pFLash->SetFlashColor(color);
//        pFLash->SetFlash(true);
        CDate mjBein(2021,4,6,12,0,0);
        Satellite::CSGP4 spg41("1 24792U 97020A 15295.80870870 .00000395 00000-0 13414-3 0  9995",
                               "2 24792 86.4009 210.8757 0002055 81.6361 278.5068 14.34222478966712");

        double dMJD = spg41.GetTLEEpoch();
        CDate temedata(dMJD);
        std::cout<<temedata;
        std::vector<Math::CVector> vPos;
        std::vector<double> vTime;
        Math::CVector vPV;

        vTime.reserve(86400);
        vPos.reserve(86400);
        for(int i=0;i<86400;i+=300)
        {
            vPV = spg41.CalPV(dMJD+i*SECDAY);
            vTime.push_back(dMJD+i*SECDAY);
            vPos.push_back(vPV);
        }

        /// 转到地固系下
        Math::CVector vECF;
        CCoorSys::TEME2ECF(dMJD,vPos[0],vECF);
        PV satPV;
        satPV.stP.dX = vECF(0);
        satPV.stP.dY = vECF(1);
        satPV.stP.dZ = vECF(2);

        satPV.stV.dX = vECF(3);
        satPV.stV.dY = vECF(4);
        satPV.stV.dZ = vECF(5);

        Pos satPRY,rPos;

        ScenePos testPos;
        Math::CVector vOther;
        GisMath::XYZ2LBH(vECF.slice(0,2),vOther);

        vOther(1) += 10*DD2R;
        vOther(2) = 0;
        //    IMapLocation* pEarthLocationTest = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
        //    testPos.fX = vOther(0)*DR2D;
        //    testPos.fY = vOther(1)*DR2D;
        //    testPos.fZ = 0;
        //    pEarthLocationTest->SetGeoPos(testPos);
        //    pEarthLocationTest->SetSceneNode(pPoint);
        //    m_pLayer->AddSceneNode(pEarthLocationTest);

        GisMath::LBH2XYZ(vOther(0),vOther(1),vOther(2),rPos.dX,rPos.dY,rPos.dZ);
        CalPRY(satPV,rPos,Rota_RYP,satPRY);
        SceneAttitude attitude;
        attitude.dRoll = satPRY.dX*DR2D;
        attitude.dPitch = satPRY.dY*DR2D;
        attitude.dYaw = satPRY.dZ*DR2D;



        pSatellite->SetJ2000Oribit(vTime,vPos);
        pSatellite->SetModelPath("Model/SJ-2/shixian-2.flt");
        m_pSceneGraph->GetMap()->GetSpaceEnv()->OpenLight(false);

        auto pSatelliteSensor = dynamic_cast<IConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IConeSensor"));
        color.fG=1.f;
        color.fA=1.f;
        pSatelliteSensor->SetAngle(1.f);
//        pSatelliteSensor->SetHAngle(.4f);
//        pSatelliteSensor->SetVAngle(1.f);
        pSatelliteSensor->SetColor(color);
        pSatellite->SetOribitColor(color);
        pSatellite->AddSensor(0, pSatelliteSensor);
        pSatellite->UpdateData(dMJD);
        m_pSceneGraph->GetMap()->GetSpaceEnv()->AddSceneNode(pSatellite);

        //m_pSceneGraph->GetMap()->UpdateDate(dMJD);

        m_pTrackNode = pSatelliteSensor;
    }
    PlotMap();
    LodPlot();
//    LoadQingxie();
}
ISceneVisualGroup* pVisualGroup =nullptr;
IMapLocation*  pMapLocation1=nullptr;
void MainWindow::on_action_2_triggered()
{
//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->HomeViewPoint();
//    return;
//    SceneColor color;
//    color.fG=color.fB=0.0f;
//    pSatellite->SetOribitColor(color);
//    SceneImageSize size;
//    size.unHeight = size.unWidth = 100;
//    pImage->SetImageSize(size);
//    pFlash->SetFlash(!pFlash->IsFlash());
//    pEarthLocation->SetVisible(!pEarthLocation->IsVisible());
//    pHudLayout->RemoveHudNode(pHudText);
//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudText);
//    pEarthLocation->SetVisible(!pEarthLocation->IsVisible());
    pVisualGroup->RemoveSceneNode(pMapLocation1);
//    SceneViewPoint viewPoint;
//    viewPoint.stPos.dX = 108.78107;
//    viewPoint.stPos.dY = 34.11611;
//    viewPoint.fDistance = 1000;
//    viewPoint.fAzimuth = 0;
//    viewPoint.fElev = 80;
//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SetViewPoint(viewPoint,15u);
}

#include <Tool/ITool.h>

class SubPointPick: public PickMessage
{
public:
    void PickID(unsigned int unID,unsigned int)
    {
//        qDebug()<<unID;
    }
};

void MainWindow::on_action_3_triggered()
{
    dynamic_cast<ISensor*>(m_pTrackNode)->SetEffects(ISensor::UNIFORM_MOTION);
    m_pSceneGraph->GetTool()->SubPickMessage(new SubPointPick);
    m_pSceneGraph->GetTool()->SelecteTool("IPointPick");
//    nTimerID = startTimer(100);
}

int g_nTest=0;
void MainWindow::on_action_4_triggered()
{
    auto pViewPort = m_pSceneGraph->GetMainWindow()->GetMainViewPoint();

    if(g_nTest > IViewPort::VERTICAL_SPLIT)
    {
        g_nTest = 0;
        pViewPort->OpenStereo(false);
    }
    else
    {
        pViewPort->SetStereoMode(IViewPort::StereoType(g_nTest));
        ++g_nTest;
        pViewPort->OpenStereo(true);
    }

}

/// 增加帧率
void MainWindow::on_action_5_triggered()
{
    m_pSceneGraph->GetMainWindow()->SetFrameRate(100);
}

/// 降低帧率
void MainWindow::on_action_6_triggered()
{
    m_pSceneGraph->GetMainWindow()->SetFrameRate(10);
}


void MainWindow::PlotMap()
{
    ScenePos pos;

    pos.dX = -121.5;
    pos.dY = 25;
    pos.dZ = 1050;
    SceneColor color;
    color.fG = .0f;
    color.fB = .0f;

    /// 绘制线
    auto m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    pos.dX = 122.8;
    pos.dY = 26.8;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.82;
    pos.dY = 26.82;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.83;
    pos.dY = 26.84;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.84;
    pos.dY = 26.83;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.84;
    pos.dY = 26.83;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.85;
    pos.dY = 26.84;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 122.85;
    pos.dY = 27;
    m_pLine->GetDrawLine()->AddPoint(pos);
    pos.dX = 123;
    pos.dY = 27.6;
    m_pLine->GetDrawLine()->AddPoint(pos);
    m_pLine->GetDrawLine()->SetColor(color);
    m_pLine->GetDrawLine()->SetLineWidth(2.f);
//    m_pLine->GetDrawLine()->OpenGlow(false);
    m_pLine->SetTerrainType(IMapSceneNode::RELATIVE_TERRAIN);
    m_pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
    pos.dX = -10;
    pos.dY = 0;
    m_pPolygon->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 10;
    pos.dY = 0;
    m_pPolygon->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 10;
    pos.dY = 27;
    m_pPolygon->GetDrawPolygon()->AddPoint(pos);
    pos.dX = -10;
    pos.dY = 27;
    m_pPolygon->GetDrawPolygon()->AddPoint(pos);
    m_pPolygon->GetDrawPolygon()->SetColor(color);
    m_pPolygon->SetTerrainType(IMapSceneNode::RELATIVE_TERRAIN);
    m_pPolygon->GetDrawPolygon()->ShowOutLine(true);
    m_pLayer->AddSceneNode(m_pPolygon);

    /// 绘制地图上的点
    auto pMapLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pLayer->AddSceneNode(pMapLocation);
    pMapLocation->SetGeoPos(pos);

    auto pBoxSensor = dynamic_cast<IBoxSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IBoxSensor"));
    pMapLocation->SetSceneNode(pBoxSensor);
    pBoxSensor->SetDistance(10000);
    color.fA = .1f;
    pBoxSensor->SetColor(color);
    pBoxSensor->ShowFace(true);
    pBoxSensor->ShowLine(true);
    pBoxSensor->SetShowBack(false);

    ///绘制地图元素
    color.fG=color.fB=1.f;
    color.fA = 1.f;
    pos.dX += 1.0f;
    auto m_pPoint = dynamic_cast<IMapPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    m_pPoint->GetDrawPoint()->AddPoint(0,pos);
    m_pPoint->GetDrawPoint()->SetColor(color);
    m_pPoint->GetDrawPoint()->SetPointSize(50.f);
    m_pPoint->GetDrawPoint()->SetImage("Image/China.png");
    m_pPoint->SetTerrainType(IMapSceneNode::ABSOLUTE_TERRAIN);
    m_pLayer->AddSceneNode(m_pPoint);

    auto m_pPolygon3D = dynamic_cast<IMapPolygon3D*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon3D"));

    pos.dX = 123;
    pos.dY = 27;
    m_pPolygon3D->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 124;
    pos.dY = 27;
    m_pPolygon3D->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 124;
    pos.dY = 28;
    m_pPolygon3D->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 123.5;
    pos.dY = 29;
    m_pPolygon3D->GetDrawPolygon()->AddPoint(pos);
    pos.dX = 123;
    pos.dY = 28;
    m_pPolygon3D->GetDrawPolygon()->AddPoint(pos);
    color.fR = 1.f;
    color.fA = 0.6f;
    m_pPolygon3D->GetDrawPolygon()->SetColor(color);
    m_pPolygon3D->GetDrawPolygon()->SetHeight(100000);
    m_pPolygon3D->SetTerrainType(IMapSceneNode::ABSOLUTE_TERRAIN);
    m_pPolygon3D->GetDrawPolygon()->ShowBottom(false);
    m_pLayer->AddSceneNode(m_pPolygon3D);

    /// 添加
    pMapLocation1 = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));

    pos.dX = 122.85;
    pos.dY = 26.84;
    pos.dZ -= 400;
    pMapLocation1->SetGeoPos(pos);
//    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("F:/BaiduNetdiskDownload/xian/Data/out.osgb",false);
//    pModel->SetCanPick(true);
    pVisualGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(VISUAL_GROUP)->AsSceneVisualGroup();
    SceneViewPoint viePoint;
    viePoint.fAzimuth = 0;
    viePoint.fDistance = 10000;
    viePoint.stPos = pos;
    viePoint.stPos.dY -= 0.005;
    pVisualGroup->SetViewPoint(viePoint);
    pVisualGroup->AddSceneNode(pMapLocation1);
//    pMapLocation1->SetSceneNode(pModel);
    m_pSceneGraph->GetRoot()->AddSceneNode(pVisualGroup);
    m_pLayer->AddSceneNode(pMapLocation1);

    ISConeSensor* pSCone = dynamic_cast<ISConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISConeSensor"));
    pSCone->SetColor(color);
    pSCone->SetHAngle(25);
    pSCone->SetVAngle(25);
    pSCone->SetDistance(viePoint.fDistance);
    pSCone->ShowLine(true);
    pSCone->ShowFace(false);

    ISceneAttitudeGroup* pAttitude = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    pAttitude->AddSceneNode(pSCone);
    SceneAttitude attitude;
    attitude.dRoll=-90;
    attitude.dYaw = -viePoint.fAzimuth;
    attitude.dPitch = -viePoint.fElev;
    pAttitude->SetAttitude(attitude);
    IMapLocation*  pMapLocation2 = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    pMapLocation2->SetGeoPos(viePoint.stPos);
    pMapLocation2->SetSceneNode(pAttitude);
    m_pLayer->AddSceneNode(pMapLocation2);

    /// clh
    ScenePos poss;
    poss.dX = 30;
    poss.dY = 30;
    poss.dZ =0;
    auto pNewImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pNewImage->SetImagePath("Image/ship.png");
    SceneImageSize stSize;
    stSize.unWidth = stSize.unHeight = 100;
    pNewImage->SetImageSize(stSize);
    auto pAutoImage1 = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage1->SetAutoScal(true);
    pAutoImage1->AddSceneNode(pNewImage);

    auto pMapLocationCLH = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pLayer->AddSceneNode(pMapLocationCLH);
    pMapLocation->SetGeoPos(poss);
    pMapLocation->SetSceneNode(pAutoImage1);


    IMapCoverImage* pCoverImage = dynamic_cast<IMapCoverImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapCoverImage"));
    ScenePos lu, rb;
    lu.dX = 114.50; lu.dY = 38.05, lu.dZ = 10;
    rb.dX = 114.52; rb.dY = 38.00, rb.dZ = 10;
    pCoverImage->SetBound(lu,rb,"D:/1.png");
    m_pLayer->AddSceneNode(pCoverImage);

}

void MainWindow::LodPlot()
{
    auto pNewImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pNewImage->SetImagePath("Image/ship.png");
    SceneImageSize stSize;
    stSize.unWidth = stSize.unHeight = 100;
    pNewImage->SetImageSize(stSize);
    auto pAutoImage1 = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage1->SetAutoScal(true);
    pAutoImage1->AddSceneNode(pNewImage);

    auto pNewImage1 = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pNewImage1->SetImagePath("Image/ship.png");
    SceneColor blueColor;
    blueColor.fB = blueColor.fR = 0.f;
    pNewImage1->SetColor(blueColor);

    auto pAutoImage2 = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage2->SetAutoScal(true);
    pAutoImage2->AddSceneNode(pNewImage1);

    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    pLod->AddSceneNode(pAutoImage1);
    pLod->AddSceneNode(pAutoImage2);

    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("Model/AirPlane.ive");
    auto pAutoModel = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoModel->SetAutoScal(true);
    pAutoModel->AddSceneNode(pModel);
    pLod->AddSceneNode(pAutoModel);

    std::vector<float> vLevelInfo;
    vLevelInfo.push_back(1e7);
    vLevelInfo.push_back(1e5);

    pLod->SetLevelsInfo(vLevelInfo);

    ScenePos pos;
    pos.dX = 122.84;
    pos.dY = 26.83;
    pos.dZ = 1000.f;
    auto pEarthLocation1 = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    pEarthLocation1->SetSceneNode(pLod);
    pEarthLocation1->SetGeoPos(pos);
    m_pLayer->AddSceneNode(pEarthLocation1);
}

void MainWindow::TestGroup()
{
    m_pTestNode = new CPlaceNode(m_pSceneGraph);
    m_pTestNode->SetPos(10,10,10);
    m_pTestNode->SetImagePath("Image/Mine/1.png");
    m_pTestNode->SetText("Hello world");
    m_pLayer->AddSceneNode(m_pTestNode->GetLocationNode());

    SceneColor color;
    color.fG = color.fB = 0.f;
    m_pTestNode->SetColor(color);

    color.fR = color.fB = 0.f;
    color.fG = 1.f;
    auto pPlaceNode = new CPlaceNode(m_pSceneGraph);
    pPlaceNode->SetPos(120.,34.,10);
    pPlaceNode->SetImagePath("Image/Mine/1.png");
    pPlaceNode->SetColor(color);
    pPlaceNode->SetText("next dfdlj");
    m_pLayer->AddSceneNode(pPlaceNode->GetLocationNode());

    m_pTestNode1 = new CPlaceNode(m_pSceneGraph);
    m_pTestNode1->SetPos(130.,34.,10);
    m_pTestNode1->SetImagePath("Image/Mine/1.png");
    m_pTestNode1->SetColor(color);
    m_pTestNode1->SetText("sdweedfeeff dfdlj");
    m_pLayer->AddSceneNode(m_pTestNode1->GetLocationNode());

//    ScenePos pos;
//    pos.fX = 120.f;
//    pos.fY = 24.f;
//    auto pEarthLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
//    auto pScreenGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCREEN_GROUP);
//    pEarthLocation->SetGeoPos(pos);
//    pEarthLocation->SetTerrainType(pEarthLocation->ABSOLUTE_TERRAIN);

//    auto pImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
//    pImage->SetImagePath("Image/ship.png");
//    pScreenGroup->AddSceneNode(pImage);

//    auto pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
//    pScreenGroup->AddSceneNode(pLabel);

//    pLabel->SetText("hello wolrd");
//    ScenePixelOffset offset;
//    offset.sHeight=0;
//    offset.sWidth=16;
//    pLabel->SetPixelOffset(offset);
//    pEarthLocation->SetSceneNode(pScreenGroup);
//    m_pLayer->AddSceneNode(pEarthLocation);
}

void MainWindow::LoadQingxie()
{
    ScenePos pos;
    pos.dX = 108.78107;
    pos.dY = 34.11611;
    pos.dZ = 100.;
    auto pEarthLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    auto pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("F:/BaiduNetdiskDownload/shancheng/shanchengzhen/Data/out.osgb",false);
    pEarthLocation->SetGeoPos(pos);
    pEarthLocation->SetSceneNode(pModel);
    m_pLayer->AddSceneNode(pEarthLocation);
}

class MjyTest:public IHudLayoutMouseObserver
{
public:
    virtual void DoubleClick(const IHudLayout*) const
    {
        std::cout<<"DoubleClick"<<std::endl;
    }
    virtual void Move(const IHudLayout*,const HudPos& pos,int nWidth, int nHeight)const
    {
        std::cout<<"Move"<<pos.nX<<','<<pos.nY<<':'<<nWidth<<','<<nHeight<<std::endl;
    }
};

void MainWindow::TestHud()
{
    SceneColor color;
    pHudLayout = dynamic_cast<IHudLayout*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudLayout"));

    pHudText = dynamic_cast<IHudText*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudText"));
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudLayout);

//    auto pHudImage = dynamic_cast<IHudImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudImage"));
//    pHudImage->SetImage("Image/ship.png");
//    pHudLayout->AddHudNode(pHudImage);
    pHudLayout->AddHudNode(pHudText);
    pHudLayout->SetPosType(HUD_DOWN_RIGHT);
    color.fR=1.0f;
    color.fG=0.0f;
    color.fB=0.0f;
    pHudText->SetText("Hello world");
    pHudText->SetColor(color);
    color.fA = 0.5f;
    color.fG = 1.0f;
    pHudLayout->SetFillColor(color);
    color.fR = color.fG = color.fB=0.f;
    pHudText->SetOutColor(color);

    ///标牌
    auto pLabel = dynamic_cast<IHudLayout*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudLayout"));
    pLabel->SetLayoutType(IHudLayout::LAYOUT_VERTICAL);
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pLabel);
    pLabel->SetPosType(HUD_FIXED);
    HudPos ScreenPos;
    ScreenPos.nX = 100;
    ScreenPos.nY = 100;
    pLabel->SetHudPos(ScreenPos);

    auto pText = dynamic_cast<IHudText*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudText"));
    pText->SetText("Label");
    color.fR=1.0f;
    color.fG=1.0f;
    color.fA = 1.0f;
    pText->SetColor(color);
    color.fR=1.0f;
    color.fG=0.0f;
    color.fB=0.0f;
    color.fA = 0.4f;
    pLabel->SetFillColor(color);
    pLabel->AddHudNode(pText);

    pText = dynamic_cast<IHudText*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudText"));
    pText->SetText("label2\n 真香");
    pText->SetFont("Fonts/msyh.ttf");
    color.fR=0.0f;
    color.fG=1.0f;
    color.fA = 1.0f;
    pText->SetFontSize(11);
    pText->SetColor(color);
    pLabel->AddHudNode(pText);
    pLabel->SubObserver(new MjyTest());

    // pLabel->SetOutLineColor(color);
}

/// 测试标绘模型
void MainWindow::on_action12_triggered()
{
    ISceneGroup* pSceneRoot = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);

//    ISceneNode * pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("D:/MyData/Tencent/BIMGIS_scz20201013/data/3dmodel/tree2.FBX",false);
//    ISceneNode * pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("F:/zhiwu.obj",false);
//    pSceneRoot->AddSceneNode(pModel);
    SceneColor color;

//    color.fR=1.0f;
    color.fG=0.0f;
    color.fB=0.0f;
    color.fA=1.f;
    /// 绘制点
    auto pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    pPoint->SetPointSize(100.f);
//    pPoint->SetImage("Image/ship.png");


    pPoint->SetColor(color);
//    pSceneRoot->AddSceneNode(pPoint);

    /// 绘制线
    auto pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    pLine->SetColor(color);

    ScenePos scenePos;
    scenePos.dZ = 10.f;
    pLine->AddPoint(0,scenePos);

    scenePos.dX = 100.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(1,scenePos);

    scenePos.dX = 100.f;
    scenePos.dY = 100.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(2,scenePos);

    scenePos.dX = 0.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(3,scenePos);
    scenePos.dY = 0.f;
    pLine->AddPoint(4,scenePos);
//    pSceneRoot->AddSceneNode(pLine);
    pLine->SetLineWidth(10);
    pLine->SetLineType(pLine->DOTTED_LINE);
    pLine->OpenGlow(true);

    /// 绘制图片
//    QImage image = QImage("E:/splash-620x300.png").convertToFormat(QImage::Format_RGBA8888);
    auto pImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pImage->SetImagePath("Image/Mine/1.png");
//    RGBAData data;
//    data.unWidth = image.width();
//    data.unHeight = image.height();
//    data.bFlipVertically=true;
//    data.pRGBAData = new unsigned char[image.byteCount()]();
//    memcpy(data.pRGBAData,image.bits(),image.byteCount());
//    pImage->SetRGBAData(data);

    color.fG=color.fB=0.0f;
    pImage->SetColor(color);

    SceneImageSize size;
    size.unHeight=56;
    size.unWidth=56;
    size.bOutSet=true;
    pImage->SetImageSize(size);


    ISceneVisualGroup* pVisualGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(VISUAL_GROUP)->AsSceneVisualGroup();
    SceneViewPoint viePoint;
    viePoint.fAzimuth = 90;
    viePoint.fDistance = 80;
    viePoint.fElev=-30;
    scenePos.dX=-28;
    scenePos.dZ=28;

    viePoint.stPos = scenePos;
    pVisualGroup->SetViewPoint(viePoint);
    pVisualGroup->SetPosIsGeo(false);
    pSceneRoot->AddSceneNode(pVisualGroup);

    ISConeSensor* pSCone = dynamic_cast<ISConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISConeSensor"));
    pSCone->SetColor(color);
    pSCone->SetHAngle(25);
    pSCone->SetVAngle(25);
    pSCone->SetDistance(viePoint.fDistance);
    pSCone->ShowLine(true);
    pSCone->ShowFace(false);

    ISceneAttitudeGroup* pAttitude = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    pAttitude->AddSceneNode(pSCone);
    pAttitude->SetPos(viePoint.stPos);
    SceneAttitude attitude;
    attitude.dRoll=-90;
    attitude.dYaw = -viePoint.fAzimuth;
    attitude.dPitch = -viePoint.fElev;
    pAttitude->SetAttitude(attitude);
//    pSceneRoot->AddSceneNode(pAttitude);
//    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
//    pLod->AddSceneNode(pAutoImage);

    ILabel* pLable = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    pLable->SetText("hello world");
    pLable->SetAttachNode(pImage);
    pSceneRoot->AddSceneNode(pImage);

    /// 绘制多边形
    auto pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon"));
//    pSceneRoot->AddSceneNode(pPolygon);
    pPolygon->SetMultPos(pLine->GetMulPos());
    scenePos.dX=50.f;
    scenePos.dY=50.f;
    pPolygon->AddPoint(pPolygon->GetCount(),scenePos);
    pPolygon->SetColor(color);
    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("E:/Git/MJY/Bin/Data/Model/AirPlane.ive",false);
    pSceneRoot->AddSceneNode(pModel);
//    pModel->SetCanPick(true);
    pVisualGroup->AddSceneNode(pModel);
//    pVisualGroup->AddSceneNode(pImage);
    IBoxSensor* pBox = dynamic_cast<IBoxSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IBoxSensor"));
    pBox->SetDistance(10);
//    IFlatAbility* pFlatAbility = pBox->GetOrCreateAbility(FLAT_ABILITY)->AsFlatAbility();
//    FlatArea area;
//    area.stLeftUp.dX = -5;
//    area.stLeftUp.dY = 5;

//    area.stRightUp.dX = 5;
//    area.stRightUp.dY = 5;

//    area.stRightDown.dX = 5;
//    area.stRightDown.dY = 0;

//    area.stLeftDown.dX = -5;
//    area.stLeftDown.dY = 0;
//    pFlatAbility->AddHole(area);
//    pBox->SetShowBack(true);
    pBox->ShowLine(true);
//    pBox->ShowFace(false);
//    pVisualGroup->AddSceneNode(pBox);
//    pSceneRoot->AddSceneNode(pBox);

    TestHud();

    m_pSceneGraph->GetRoot()->AddSceneNode(pSceneRoot);
}

void MainWindow::on_BeginCapture_triggered()
{
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->BeginCapture();
}

void MainWindow::on_endCapture_triggered()
{
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->EndCapture();
}

void MainWindow::on_action_OpenVR_triggered()
{
#ifdef NEED_VR
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->ShowOnVR();
#endif
}


void MainWindow::on_action_CloseVR_triggered()
{
#ifdef NEED_VR
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->ShutDownVR();
#endif
}

