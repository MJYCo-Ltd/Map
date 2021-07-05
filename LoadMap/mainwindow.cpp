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
#include <Plot/SceneShape/IBoxSensor.h>
#include <Hud/IViewHud.h>
#include <Inner/ILoadResource.h>
#include <Plot/Map/IMapObserver.h>
#include <Ability/IFlashAbility.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ExternShape/MapRectange.h>
#include <ExternShape/MapCircle.h>

struct MapObser:public IMapMessageObserver,public IWindowMessageObserver
{
    MapObser(ISceneGraph* pSceneGraph):m_pSceneGraph(pSceneGraph){}

    void MousePos(float fLon,float fLat,float fHeight)
    {
        pos.fX = fLon;
        pos.fY = fLat;
        pos.fZ = fHeight;
    }

    void MovePos(const ScenePos& wordPos)
    {
//        std::cout<<wordPos.fX<<','<<wordPos.fY<<','<<wordPos.fZ<<std::endl;
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
        m_pLayer = m_pSceneGraph->GetMap()->CreateLayer("Test");
        auto pMap= new MapObser(m_pSceneGraph);
        m_pSceneGraph->GetMainWindow()->SubMessage(pMap);
        m_pSceneGraph->GetMap()->SubMessage(pMap);
//        m_pSceneGraph->GetMap()->GetSpaceEnv()->ShowSpaceBackGround(false);
//        m_pSceneGraph->GetMap()->OpenLight(false);
//        m_pSceneGraph->GetMap()->SetShowAtmosphere(true);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
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
IImage* pImage=nullptr;

void MainWindow::on_action_triggered()
{
    m_pSceneGraph->GetMap()->OpenLight(true);
    ScenePos pos11;
    pos11.fX = 126.0;
    pos11.fY = 45.6;
    pos11.fZ = 10;

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




    QImage image = QImage("E:/splash-620x300.png").convertToFormat(QImage::Format_RGBA8888);
    pImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
//    pImage->SetImagePath("Image/Mine/17.png");
    RGBAData data;
    data.unWidth = image.width();
    data.unHeight = image.height();
    data.bFlipVertically=true;
    data.pRGBAData = new unsigned char[image.byteCount()]();
    memcpy(data.pRGBAData,image.bits(),image.byteCount());
    pImage->SetRGBAData(data);
    SceneColor color;
    color.fG=color.fB=.0f;
    pImage->SetColor(color);

    SceneImageSize size;
    size.unHeight=32;
    size.unWidth=32;
    size.bOutSet=true;
    pImage->SetImageSize(size);

    ISceneGroup* pSceneRoot = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);

    auto pAutoImage = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage->SetMinScal(1.);
    pAutoImage->AddSceneNode(pImage);

    pHudLayout = dynamic_cast<IHudLayout*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudLayout"));

    pHudText = dynamic_cast<IHudText*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudText"));
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudLayout);

//    auto pHudImage = dynamic_cast<IHudImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IHudImage"));
//    pHudImage->SetImage("Image/ship.png");
//    pHudLayout->AddHudNode(pHudImage);
    pHudLayout->AddHudNode(pHudText);
    pHudLayout->SetPosType(HUD_DOWN_RIGHT);
//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->GetHud()->AddHudNode(pHudImage);

    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    pLod->AddSceneNode(pAutoImage);


    color.fR=0.0f;
    color.fG=0.0f;
    color.fB=1.0f;
    pHudText->SetText("Hello world");
    pHudText->SetColor(color);

//    color.fG=color.fB = 1.f;
    color.fR = color.fG = color.fB=0.f;
    pHudText->SetOutColor(color);
    color.fG = 1.f;
//    return;


    pSceneRoot->AddSceneNode(pLod);

    if(MAP_3D == m_pSceneGraph->GetMap()->GetMapType())
    {
        /// 标绘卫星
        pSatellite= dynamic_cast<ISatellite*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ISatellite"));

        pSatellite->SetName("卫星");
        pSatellite->SetScale(10000.);
        IFlashAbility* pFLash = pSatellite->GetOrCreateAbility(FLASH_ABILITY)->AsFlashAbility();
        pFLash->SetFlashColor(color);
        CDate mjBein(2021,4,6,12,0,0);
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

        ScenePos testPos;
        CVector vOther;
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

        m_pSceneGraph->GetMap()->UpdateDate(dMJD);

        m_pTrackNode = pSatelliteSensor;
    }
    PlotMap();
//    LodPlot();
    TestGroup();
    LoadQingxie();
}

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
    SceneViewPoint viewPoint;
    viewPoint.stPos.fX = 108.78107;
    viewPoint.stPos.fY = 34.11611;
    viewPoint.fDistance = 1000;
    viewPoint.fAzimuth = 0;
    viewPoint.fElev = 80;
    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SetViewPoint(viewPoint,15u);
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

#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapPoint.h>
#include <Plot/Map/IMapPolygon.h>

void MainWindow::PlotMap()
{
    ScenePos pos;
    ///绘制地图元素
    auto m_pPoint = dynamic_cast<IMapPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    pos.fX = -121.5;
    pos.fY = 25;
    pos.fZ = 1000;
    m_pPoint->GetDrawPoint()->AddPoint(0,pos);
    SceneColor color;
    color.fG = .0f;
    color.fB = .0f;
    m_pPoint->GetDrawPoint()->SetColor(color);
    m_pPoint->GetDrawPoint()->SetPointSize(50.f);
    m_pPoint->GetDrawPoint()->SetImage("Image/ship.png");
    m_pPoint->SetTerrainType(IMapSceneNode::ABSOLUTE_TERRAIN);
    m_pLayer->AddSceneNode(m_pPoint);

    /// 绘制线
    auto m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    m_pLine->GetDrawLine()->AddPoint(0,pos);
    pos.fX = -92;
    pos.fY = 26;
    m_pLine->GetDrawLine()->AddPoint(1,pos);
    pos.fX = -45;
    pos.fY = 45;
    m_pLine->GetDrawLine()->AddPoint(2,pos);
    pos.fX = 0;
    pos.fY = 60;
    m_pLine->GetDrawLine()->AddPoint(3,pos);
    pos.fX = 10;
    pos.fY = 45;
    m_pLine->GetDrawLine()->AddPoint(4,pos);
    pos.fX = 40;
    pos.fY = 35;
    m_pLine->GetDrawLine()->AddPoint(5,pos);
    pos.fX = 123;
    pos.fY = 27;
    m_pLine->GetDrawLine()->AddPoint(6,pos);
    pos.fX = 170;
    pos.fY = 25;
    m_pLine->GetDrawLine()->AddPoint(7,pos);
    m_pLine->GetDrawLine()->SetColor(color);
    m_pLine->GetDrawLine()->SetLineWidth(20.f);
    m_pLine->GetDrawLine()->OpenGlow(true);
    m_pLine->SetTerrainType(IMapSceneNode::ABSOLUTE_TERRAIN);
    m_pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
    m_pPolygon->GetDrawPolygon()->AddPoint(0,pos);
    pos.fX = 123;
    pos.fY = 26;
    m_pPolygon->GetDrawPolygon()->AddPoint(1,pos);
    pos.fX = 121;
    pos.fY = 26;
    m_pPolygon->GetDrawPolygon()->AddPoint(2,pos);
    pos.fX = 122;
    pos.fY = 26.5;
    m_pPolygon->GetDrawPolygon()->AddPoint(3,pos);
    pos.fX = 121;
    pos.fY = 27;
    m_pPolygon->GetDrawPolygon()->AddPoint(4,pos);
    m_pPolygon->GetDrawPolygon()->SetColor(color);
    m_pPolygon->SetTerrainType(IMapSceneNode::ABSOLUTE_TERRAIN);
    m_pLayer->AddSceneNode(m_pPolygon);

    auto pMapLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    m_pLayer->AddSceneNode(pMapLocation);
    pMapLocation->SetGeoPos(pos);

    auto pBoxSensor = dynamic_cast<IBoxSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IBoxSensor"));
    pMapLocation->SetSceneNode(pBoxSensor);
    pBoxSensor->SetDistance(10000);
    color.fA = 0.1f;
    pBoxSensor->SetColor(color);
    pBoxSensor->ShowFace(true);
    pBoxSensor->ShowLine(true);
    pBoxSensor->SetShowBack(false);
}

void MainWindow::LodPlot()
{
    auto pNewImage = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pNewImage->SetImagePath("Image/China.png");
    auto pAutoImage1 = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage1->SetMinScal(1.);
    pAutoImage1->AddSceneNode(pNewImage);

    auto pNewImage1 = dynamic_cast<IImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IImage"));
    pNewImage1->SetImagePath("Image/ship.png");
    auto pAutoImage2 = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoImage2->SetMinScal(1.);
    pAutoImage2->AddSceneNode(pNewImage1);

    auto pLod = m_pSceneGraph->GetPlot()->CreateSceneGroup(LOD_GROUP)->AsSceneLodGroup();
    pLod->AddSceneNode(pAutoImage1);
    pLod->AddSceneNode(pAutoImage2);

    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("D:/MyData/Tencent/BIMGIS_scz20201013/data/3dmodel/tree2.FBX.ass",false);
    auto pAutoModel = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP)->AsSceneScaleGroup();
    pAutoModel->SetMinScal(1.);
    pAutoModel->AddSceneNode(pModel);
    pLod->AddSceneNode(pAutoModel);

    std::vector<float> vLevelInfo;
    vLevelInfo.push_back(1e7);
    vLevelInfo.push_back(1e5);

    pLod->SetLevelsInfo(vLevelInfo);

    ScenePos pos;
    pos.fX = 121.f;
    pos.fY = 27.f;
    pos.fZ = 1000.f;
    auto pEarthLocation1 = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    pEarthLocation1->SetSceneNode(pLod);
    pEarthLocation1->SetGeoPos(pos);
    m_pLayer->AddSceneNode(pEarthLocation1);
}

#include <ExternShape/MapPlaceNode.h>
void MainWindow::TestGroup()
{
    CPlaceNode* pPlaceNode = new CPlaceNode(m_pSceneGraph);
    pPlaceNode->SetPos(120.,24.);
    pPlaceNode->SetImagePath("Image/ship.png");
    pPlaceNode->SetText("Hello world");
    m_pLayer->AddSceneNode(pPlaceNode->GetLocationNode());

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
    pos.fX = 108.78107;
    pos.fY = 34.11611;
    pos.fZ = 100.;
    auto pEarthLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    auto pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("F:/BaiduNetdiskDownload/shancheng/shanchengzhen/Data/out.osgb",false);
    pEarthLocation->SetGeoPos(pos);
    pEarthLocation->SetSceneNode(pModel);
    m_pLayer->AddSceneNode(pEarthLocation);
}

/// 测试标绘模型
void MainWindow::on_action12_triggered()
{
    ISceneGroup* pSceneRoot = m_pSceneGraph->GetPlot()->CreateSceneGroup(STANDARD_GROUP);

//    ISceneNode * pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("D:/MyData/Tencent/BIMGIS_scz20201013/data/3dmodel/tree2.FBX",false);
    ISceneNode * pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("F:/zhiwu.obj",false);
    pSceneRoot->AddSceneNode(pModel);
    SceneColor color;

    color.fR=1.0f;
    color.fG=0.0f;
    color.fB=0.0f;

    /// 绘制点
    auto pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    pPoint->SetPointSize(50.f);
    pPoint->SetImage("Image/China.png");


    pPoint->SetColor(color);
//    pSceneRoot->AddSceneNode(pPoint);

    /// 绘制线
    auto pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    pLine->SetColor(color);

    ScenePos scenePos;
    scenePos.fZ = 10.f;
    pLine->AddPoint(0,scenePos);

    scenePos.fX = 100.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(1,scenePos);

    scenePos.fX = 100.f;
    scenePos.fY = 100.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(2,scenePos);

    scenePos.fX = 0.f;
    pPoint->AddPoint(pPoint->GetCount(),scenePos);
    pLine->AddPoint(3,scenePos);
    pSceneRoot->AddSceneNode(pLine);
    pLine->SetLineWidth(20);
    pLine->OpenGlow(true);

    /// 绘制多边形
    auto pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon"));
//    pSceneRoot->AddSceneNode(pPolygon);
    pPolygon->SetMultPos(pLine->GetMulPos());
    scenePos.fX=50.f;
    scenePos.fY=50.f;
    pPolygon->AddPoint(pPolygon->GetCount(),scenePos);
    pPolygon->SetColor(color);
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

