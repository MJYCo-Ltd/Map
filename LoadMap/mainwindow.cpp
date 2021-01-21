#include <QFileDialog>
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
#include <Plot/SceneShape/IRadarSensor.h>
#include <Plot/SatelliteShow/ISatellite.h>
#include <Plot/Hud/IHudText.h>
#include <Plot/Hud/IHudImage.h>
#include <Plot/Hud/IHudLayout.h>
#include <Plot/SceneShape/IImage.h>
#include <Plot/Common/ISceneScaleGroup.h>
#include <Plot/Common/ISceneLodGroup.h>
#include <Plot/SceneShape/ILabel.h>
#include <Hud/IViewHud.h>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSecenGraph(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
//    ScenePos pos;
//    pos.bIsGeo = true;
//    pos.fHeight = 500;
//    ++m_nTimes;
//    double dLon,dLat;
//    if(m_nTimes < 2000)
//    {
//        GisMath::CalBaiser(121.225100*DD2R,23.128880*DD2R,dA1,m_nTimes*dL1,dLon,dLat);
//        pos.fLon = dLon*DR2D;
//        pos.fLat = dLat*DR2D;
//        m_pModel->SetPos(pos);

//        m_pModel->SetYPR(dA1*DR2D,0,0);
//    }
//    else if(m_nTimes < 4000)
//    {
//        GisMath::CalBaiser(121.185947*DD2R,23.123019*DD2R,dA2,(m_nTimes-2000)*dL2,dLon,dLat);
//        pos.fLon = dLon*DR2D;
//        pos.fLat = dLat*DR2D;
//        m_pModel->SetPos(pos);
//        m_pModel->SetYPR(dA2*DR2D,0,0);
//    }
//    else if(m_nTimes < 6000)
//    {
//        GisMath::CalBaiser(121.178775*DD2R,23.101700*DD2R,dA3,(m_nTimes-4000)*dL3,dLon,dLat);
//        pos.fLon = dLon*DR2D;
//        pos.fLat = dLat*DR2D;
//        m_pModel->SetPos(pos);
//        m_pModel->SetYPR(dA3*DR2D,0,0);
//    }
//    else
//    {
//        killTimer(nTimerID);
//    }
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
IImage* pImage=nullptr;

void MainWindow::on_action_triggered()
{
    pImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pImage->SetImagePath("Images/ship.png");

    ISceneGroup* pSceneRoot = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);

    auto pAutoImage = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage->SetMinScal(1.);
    pAutoImage->AddSceneNode(pImage);

    pHudLayout = dynamic_cast<IHudLayout*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudLayout"));

    pHudText = dynamic_cast<IHudText*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudText"));
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudLayout);

    auto pHudImage = dynamic_cast<IHudImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudImage"));
    pHudImage->SetImage("Images/ship.png");
    pHudLayout->AddHudNode(pHudImage);
    pHudLayout->AddHudNode(pHudText);
    pHudLayout->SetPosType(HUD_CENTER_CENTER);
//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudImage);

    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    pLod->AddSceneNode(pAutoImage);

    SceneColor color;
    color.fR=1.0f;
    color.fG=1.0f;
    color.fB=1.0f;
    pHudText->SetText("Hello world");
    pHudText->SetColor(color);

//    color.fG=color.fB = 1.f;
    color.fR = color.fG = color.fB=0.f;
    pHudText->SetOutColor(color);
    color.fG = 1.f;
//    return;


    pSceneRoot->AddSceneNode(pLod);

    /// 绘制点
    auto pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint1 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint2 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint3 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    pPoint->SetPointSize(5.f);
    pPoint1->SetPointSize(5.f);
    pPoint2->SetPointSize(5.f);
    pPoint3->SetPointSize(5.f);


    pPoint->SetColor(color);
    pPoint1->SetColor(color);
    pPoint2->SetColor(color);
    pPoint3->SetColor(color);
    pSceneRoot->AddSceneNode(pPoint);
    pSceneRoot->AddSceneNode(pPoint1);
    pSceneRoot->AddSceneNode(pPoint2);
    pSceneRoot->AddSceneNode(pPoint3);

    /// 绘制线
    auto pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    pLine->SetColor(color);

    ScenePos scenePos;
    pLine->AddPoint(0,scenePos);

    scenePos.fX = 100.f;
    pPoint1->SetPos(scenePos);
    pLine->AddPoint(1,scenePos);

    scenePos.fX = 100.f;
    scenePos.fY = 100.f;
    pPoint2->SetPos(scenePos);
    pLine->AddPoint(2,scenePos);

    scenePos.fX = 0.f;
    pPoint3->SetPos(scenePos);
    pLine->AddPoint(3,scenePos);
    //pSceneRoot->AddSceneNode(pLine);
    pLine->SetLineWidth(2);

    /// 绘制多边形
    auto pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon"));
    pPolygon->SetMultPos(pLine->GetMulPos());
    scenePos.fX=50.f;
    scenePos.fY=50.f;
    pPolygon->AddPoint(4,scenePos);
    pPolygon->SetColor(color);
//    pSceneRoot->AddSceneNode(pPolygon);


    /// 绘制锥形波
    auto pConSensor = dynamic_cast<IConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IConeSensor"));
    pConSensor->SetDistance(10.);
    color.fG=1.f;
    color.fA=.6f;
    pConSensor->SetAngle(50.f);
    pConSensor->SetColor(color);
    pSceneRoot->AddSceneNode(pConSensor);

    /// 绘制方波
    auto pAttitudeGroup = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    auto pSConSensor = dynamic_cast<ISConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISConeSensor"));
    pSConSensor->SetDistance(100.);
    color.fG=0.f;
    color.fB=1.f;
    color.fA=0.3f;
    pSConSensor->SetColor(color);
    //pSConSensor->ShowLine(false);
    pSConSensor->SetHAngle(4.f);
    pSConSensor->SetVAngle(10.f);
    pAttitudeGroup->AddSceneNode(pSConSensor);
    pAttitudeGroup->SetPos(scenePos);

    SceneAttitude attitue;
    attitue.dPitch = 90;
    pAttitudeGroup->SetAttitude(attitue);
    pSceneRoot->AddSceneNode(pAttitudeGroup);

    /// 绘制雷达波
    /// 绘制方波
    auto pAttitudeGroup1 = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    auto pRadarSensor = dynamic_cast<IRadarSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IRadarSensor"));
    pAttitudeGroup1->AddSceneNode(pRadarSensor);
    pAttitudeGroup1->SetPos(pPoint2->Pos());
    pRadarSensor->SetDistance(100.);
    pRadarSensor->SetAzimuth(0,150);
    pRadarSensor->SetElevation(-10,10);
    pRadarSensor->SetColor(color);
    //pRadarSensor->ShowFace(false);
    pSceneRoot->AddSceneNode(pAttitudeGroup1);
    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("model/AirPlane.ive");
//    IMapLocation* pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    auto pScal = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pFlash = m_pSceneGraph->GetPlot()->CreateSceneGroup(FLASH_GROUP)->AsSceneFlashGroup();
    pScal->AddSceneNode(pFlash);
    pFlash->AddSceneNode(pModel);
    pFlash->SetFlash(true);
    pFlash->SetFlashFreq(0.5);
    pFlash->SetFlashColor(color);
    pLod->AddSceneNode(pScal);

    ILabel* pLabel = dynamic_cast<ILabel*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILabel"));
    pLabel->SetAttachNode(pModel);
    pLabel->SetFont("fonts/msyh.ttf");
    pLabel->SetText("飞机");

    auto pAttitudeAirplane = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    pAttitudeAirplane->SetPos(pPoint1->Pos());
    pAttitudeAirplane->AddSceneNode(pModel);
    pSceneRoot->AddSceneNode(pAttitudeAirplane);


    std::vector<float> vLevelInfo;
    vLevelInfo.push_back(1e6);

    pLod->SetLevelsInfo(vLevelInfo);
//    m_pSceneGraph->GetRoot()->AddSceneNode(pSceneRoot);
//    return;
    m_pLayer = m_pSceneGraph->GetMap()->CreateLayer("test");
    pEarthLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pLayer->AddSceneNode(pEarthLocation);
    pEarthLocation->SetSceneNode(pSceneRoot);

//    m_pTrackNode = pPoint1;

//////////////////////测试地图上的模型///////////////////////////////////////////////////
//    auto pAirPlane = m_pSceneGraph->GetPlot()->LoadSceneNode("model/AirPlane.ive");

//    auto pScal = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP);
//    pScal->AddSceneNode(pAirPlane);
//    auto pFlash = m_pSceneGraph->GetPlot()->CreateSceneGroup(FLASH_GROUP);
//    pFlash->AddSceneNode(pScal);
//    m_pSceneGraph->GetRoot()->AddSceneNode(pFlash);
//    return;
//    m_pSceneGraph->GetMap()->GetSpaceEnv()->ShowSpaceBackGround(bShowBackGround);
    bShowBackGround = !bShowBackGround;
    double dAizm2;
    GisMath::CalBaiserF(121.225100*DD2R,23.128880*DD2R,121.185947*DD2R,23.123019*DD2R,dA1,dAizm2,dL1);
    dL1 /= 2000.;
    GisMath::CalBaiserF(121.185947*DD2R,23.123019*DD2R,121.178775*DD2R,23.101700*DD2R,dA2,dAizm2,dL2);
    dL2 /= 2000.;
    GisMath::CalBaiserF(121.178775*DD2R,23.101700*DD2R,121.164617*DD2R,23.059267*DD2R,dA3,dAizm2,dL3);
    dL3 /= 2000.;

    MapGeoPos pos;

    color.fR=1.f;
    color.fG=1.f;
    color.fB=0.f;
    color.fA=1.f;

    /// 添加线
    m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    pos.fLon = 103.566212;
    pos.fLat = 19.573845;
    pos.fHeight = 500;

    m_pLine->AddPoint(0,pos);
    pos.fLon = 110.0606607;
    pos.fLat = 25.910152;

    m_pLine->AddPoint(1,pos);
    pos.fLon = 116.358510;
    pos.fLat = 20.009330;

    m_pLine->AddPoint(2,pos);
    pos.fLon = 125.625319;
    pos.fLat = 19.748377;

    m_pLine->AddPoint(3,pos);
    m_pLine->SetLineColor(color);
//    m_pLine->SetLineWidth(100.f);
    //m_pLine->SetTerrainType(RELATIVE_TERRAIN);
    m_pLayer->AddSceneNode(m_pLine);

    IMapCoverImage* pCover = dynamic_cast<IMapCoverImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapCoverImage"));
//    if(nullptr != pCover)
//    {
//        QString sFileName = QFileDialog::getOpenFileName(nullptr,QString::fromUtf8("打开xyz文件"),QApplication::applicationDirPath(),"*.xyz");

//        if(!sFileName.isNull())
//        {
//            QFile file(sFileName);
//            if(file.open(QIODevice::ReadOnly))
//            {
//                QTextStream ssIn(&file);

//                double dPreLon(-300),dMinValue(DBL_MAX),dMaxValue(DBL_MIN);

//                CoverInfo tmpCoverInfo;
//                PosValue tmpPos;

//                int nX(1),nY(0);

//                while (!ssIn.atEnd())
//                {
//                    ssIn>>tmpPos.dX>>tmpPos.dY>>tmpPos.dValue;
//                    if(0 == tmpPos.dX)
//                    {
//                        break;
//                    }

//                    tmpCoverInfo.vPosInfo.push_back(tmpPos);

//                    if(tmpPos.dX < dPreLon)
//                    {
//                        ++nX;
//                    }

//                    ++nY;

//                    dPreLon = tmpPos.dX;

//                    if(tmpPos.dValue < dMinValue)
//                    {
//                        dMinValue = tmpPos.dValue;
//                    }

//                    if(tmpPos.dValue > dMaxValue)
//                    {
//                        dMaxValue = tmpPos.dValue;
//                    }
//                }

//                tmpCoverInfo.nXNum = nX;
//                tmpCoverInfo.nYNum = nY/nX;
//                tmpCoverInfo.dMin = dMinValue;
//                tmpCoverInfo.dMax = dMaxValue;



//                pCover->SetCoverInfo(tmpCoverInfo);
//                pLayer->AddSceneNode(pCover);
//                file.close();
//            }

//        }
//    }


    /// 添加模型
    IMapLocation* pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    pLocation->SetSceneNode(pFlash);

    pos.fLon=125.625319;
    pos.fLat=19.748377;
    pLocation->SetGeoPos(pos);
    m_pLayer->AddSceneNode(pLocation);
    pos.fLat=20.748377;
    pEarthLocation->SetGeoPos(pos);

    /// 标绘卫星
    ISatellite* pSatellite= dynamic_cast<ISatellite*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISatellite"));

    pSatellite->SetName("长江");
    pSatellite->SetScale(10000.);
    CDate mjBein(2021,1,7,17,29,0);
    Satellite::CSGP4 spg41("1 91001U          20061.66666667 -.00000001  00000-0 -13106-2 0 00008",
                          "2 91001 045.0073 000.0048 0004655 268.5152 091.4846 07.15404217000017");

    double dMJD = mjBein.GetMJD();
    vector<CVector> vPos;
    vector<double> vTime;
    CVector vPV;

    vTime.reserve(86400);
    vPos.reserve(86400);
    for(int i=0;i<86400;i+=300)
    {
        vPV = spg41.CalPV(dMJD+i*SECDAY);
        vTime.push_back(dMJD+i*SECDAY);
        vPos.push_back(vPV);
    }

    /// 转到地固系下
    CVector vECF;
    CCoorSys::TEME2ECF(dMJD,vPos[0],vECF);
    PV satPV;
    satPV.stP.dX = vECF(0);
    satPV.stP.dY = vECF(1);
    satPV.stP.dZ = vECF(2);

    satPV.stV.dX = vECF(3);
    satPV.stV.dY = vECF(4);
    satPV.stV.dZ = vECF(5);

    Pos satPRY,rPos;

    MapGeoPos testPos;
    CVector vOther;
    GisMath::XYZ2LBH(vECF.slice(0,2),vOther);

    vOther(1) += 10*DD2R;
    vOther(2) = 0;
    IMapLocation* pEarthLocationTest = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    testPos.fLon = vOther(0)*DR2D;
    testPos.fLat = vOther(1)*DR2D;
    testPos.fHeight = 0;
    pEarthLocationTest->SetGeoPos(testPos);
    pEarthLocationTest->SetSceneNode(pPoint);
    m_pLayer->AddSceneNode(pEarthLocationTest);

    GisMath::LBH2XYZ(vOther(0),vOther(1),vOther(2),rPos.dX,rPos.dY,rPos.dZ);
    CalPRY(satPV,rPos,Rota_RYP,satPRY);
    SceneAttitude attitude;
    attitude.dRoll = satPRY.dX*DR2D;
    attitude.dPitch = satPRY.dY*DR2D;
    attitude.dYaw = satPRY.dZ*DR2D;



    pSatellite->SetJ2000Oribit(vTime,vPos);
    pSatellite->SetModelPath("model/SJ-2/shixian-2.flt");

    auto pSatelliteSensor = dynamic_cast<ISConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISConeSensor"));
    color.fG=1.f;
    color.fA=.6f;
    pSatelliteSensor->SetHAngle(.4f);
    pSatelliteSensor->SetVAngle(1.f);
    pSatelliteSensor->SetColor(color);
    pSatellite->SetOribitColor(color);
    pSatellite->AddSensor(0, pSatelliteSensor);
    pSatellite->UpdateData(dMJD);
    m_pSceneGraph->GetMap()->GetSpaceEnv()->AddSceneNode(pSatellite);
    m_pSceneGraph->GetMap()->GetSpaceEnv()->UpdateDate(dMJD);

    m_pTrackNode = pRadarSensor;
    PlotMap();
}

void MainWindow::on_action_2_triggered()
{
    SceneImageSize size;
    size.unHeight = size.unWidth = 100;
    pImage->SetImageSize(size);
    pFlash->SetFlash(!pFlash->IsFlash());
//    pEarthLocation->SetVisible(!pEarthLocation->IsVisible());
    pHudLayout->RemoveHudNode(pHudText);
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudText);
//    pEarthLocation->SetVisible(!pEarthLocation->IsVisible());
    SceneViewPoint viewPoint;
    viewPoint.stPos.fX = 123;
    viewPoint.stPos.fY = 26;
    viewPoint.fDistance = 3000;
    viewPoint.fAzimuth = 300;
    viewPoint.fElev = 10;
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SetViewPoint(viewPoint);
}

void MainWindow::on_action_3_triggered()
{
    nTimerID = startTimer(100);
}

void MainWindow::on_action_4_triggered()
{
    if(-1 != nTimerID)
    {
        killTimer(nTimerID);
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

#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapPoint.h>
#include <Plot/Map/IMapPolygon.h>

void MainWindow::PlotMap()
{
    MapGeoPos pos;
    ///绘制地图元素
    auto m_pPoint = dynamic_cast<IMapPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    pos.fLon = 121;
    pos.fLat = 25;
    pos.fHeight = 0;
    m_pPoint->SetGeoPos(pos);
    SceneColor color;
    color.fG = .0f;
    color.fB = .0f;
    m_pPoint->SetPointColor(color);
    m_pLayer->AddSceneNode(m_pPoint);

    /// 绘制线
    auto m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    m_pLine->AddPoint(0,pos);
    pos.fLon = 122;
    pos.fLat = 26;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 27;
    m_pLine->AddPoint(0,pos);
    m_pLine->SetLineColor(color);
    m_pLine->SetLineWidth(2.f);
    m_pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
    m_pPolygon->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 26;
    m_pPolygon->AddPoint(1,pos);
    pos.fLon = 121;
    pos.fLat = 26.5;
    m_pPolygon->AddPoint(2,pos);
    m_pPolygon->SetPolygonColor(color);
    m_pLayer->AddSceneNode(m_pPolygon);
}
