#include <QFileDialog>
#include <QDebug>

#include <SceneGraph/ISceneGraph.h>
#include <Map/IMap.h>
#include <SpaceEnv/ISpaceEnv.h>
#include <SpaceEnv/ISpaceBackGround.h>
#include <Plot/IPlot.h>
#include <Plot/MapShape/IMapLine.h>
#include <Plot/SceneShape/ILine.h>
#include <Plot/MapShape/IMapCoverImage.h>
#include <Plot/Common/ISceneModel.h>
#include <Plot/MapShape/IMapLocation.h>
#include <Plot/SceneShape/IPoint.h>
#include <Plot/SceneShape/IConeSensor.h>
#include <Plot/SceneShape/ISConeSensor.h>
#include <Plot/Common/ISceneFlashGroup.h>
#include <Plot/SceneShape/IRadarSensor.h>
#include <GisMath/GisMath.h>
#include <Sofa/sofam.h>

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
void MainWindow::on_action_triggered()
{
    /// 绘制点
    auto pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint1 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint2 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    auto pPoint3 = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    pPoint->SetPointSize(5.f);
    pPoint1->SetPointSize(5.f);
    pPoint2->SetPointSize(5.f);
    pPoint3->SetPointSize(5.f);

    SceneColor color;
    color.fR=1.0f;
    pPoint->SetColor(color);
    pPoint1->SetColor(color);
    pPoint2->SetColor(color);
    pPoint3->SetColor(color);
    m_pSceneGraph->GetRoot()->AddSceneNode(pPoint);
    m_pSceneGraph->GetRoot()->AddSceneNode(pPoint1);
    m_pSceneGraph->GetRoot()->AddSceneNode(pPoint2);
    m_pSceneGraph->GetRoot()->AddSceneNode(pPoint3);

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
    m_pSceneGraph->GetRoot()->AddSceneNode(pLine);


    /// 绘制锥形波
    auto pConSensor = dynamic_cast<IConeSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IConeSensor"));
    pConSensor->SetDistance(10.);
    color.fG=1.f;
    color.fA=.6f;
    pConSensor->SetAngle(50.f);
    pConSensor->SetColor(color);
    m_pSceneGraph->GetRoot()->AddSceneNode(pConSensor);

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
    m_pSceneGraph->GetRoot()->AddSceneNode(pAttitudeGroup);

    /// 绘制雷达波
    /// 绘制方波
    auto pAttitudeGroup1 = m_pSceneGraph->GetPlot()->CreateSceneGroup(ATTITUDE_GROUP)->AsSceneAttitudeGroup();
    auto pRadarSensor = dynamic_cast<IRadarSensor*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IRadarSensor"));
    pAttitudeGroup1->AddSceneNode(pRadarSensor);
    pAttitudeGroup1->SetPos(pPoint2->Pos());
    pRadarSensor->SetDistance(100.);
//    pRadarSensor->SetAzimuth(0,150);
//    pRadarSensor->SetElevation(-10,10);
    pRadarSensor->SetColor(color);
    //pRadarSensor->ShowFace(false);
    m_pSceneGraph->GetRoot()->AddSceneNode(pAttitudeGroup1);
    return;
//    auto pAirPlane = m_pSceneGraph->GetPlot()->LoadSceneNode("model/AirPlane.ive");

//    auto pScal = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP);
//    pScal->AddSceneNode(pAirPlane);
//    auto pFlash = m_pSceneGraph->GetPlot()->CreateSceneGroup(FLASH_GROUP);
//    pFlash->AddSceneNode(pScal);
//    m_pSceneGraph->GetRoot()->AddSceneNode(pFlash);
//    return;
    m_pSceneGraph->GetMap()->GetSpaceEnv()->ShowSpaceBackGround(bShowBackGround);
    bShowBackGround = !bShowBackGround;
    auto pLayer = m_pSceneGraph->GetMap()->CreateLayer("test");
    double dAizm2;
    GisMath::CalBaiserF(121.225100*DD2R,23.128880*DD2R,121.185947*DD2R,23.123019*DD2R,dA1,dAizm2,dL1);
    dL1 /= 2000.;
    GisMath::CalBaiserF(121.185947*DD2R,23.123019*DD2R,121.178775*DD2R,23.101700*DD2R,dA2,dAizm2,dL2);
    dL2 /= 2000.;
    GisMath::CalBaiserF(121.178775*DD2R,23.101700*DD2R,121.164617*DD2R,23.059267*DD2R,dA3,dAizm2,dL3);
    dL3 /= 2000.;

    MapGeoPos pos;

    color.fR=1.0f;

    /// 添加线
    m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    pos.fLon = 103.566212;
    pos.fLat = 19.573845;
    pos.fHeight = 500;

    m_pLine->AddPoint(0,pos);
    pos.fLon = 110.0606607;
    pos.fLat = 19.910152;

    m_pLine->AddPoint(1,pos);
    pos.fLon = 116.358510;
    pos.fLat = 20.009330;

    m_pLine->AddPoint(2,pos);
    pos.fLon = 125.625319;
    pos.fLat = 19.748377;

    m_pLine->AddPoint(3,pos);
    m_pLine->SetLineColor(color);
    //m_pLine->SetTerrainType(RELATIVE_TERRAIN);
    pLayer->AddSceneNode(m_pLine);

//    IMapCoverImage* pCover = dynamic_cast<IMapCoverImage*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapCoverImage"));
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
    ISceneNode *pModel = m_pSceneGraph->GetPlot()->LoadSceneNode("model/AirPlane.ive");
    IMapLocation* pLocation = dynamic_cast<IMapLocation*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLocation"));
    auto pScal = m_pSceneGraph->GetPlot()->CreateSceneGroup(SCALE_GROUP);
    auto pFlash = m_pSceneGraph->GetPlot()->CreateSceneGroup(FLASH_GROUP)->AsSceneFlashGroup();

    pFlash->SetFlashFreq(0.5f);
    pFlash->SetFlashColor(color);
    pScal->AddSceneNode(pModel);
    pFlash->AddSceneNode(pScal);
    pLocation->SetSceneNode(pFlash);

    pos.fLon=125.625319;
    pos.fLat=19.748377;
    pLocation->SetGeoPos(pos);
    pLayer->AddSceneNode(pLocation);

//    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SetTrackNode(m_pModel);
}

void MainWindow::on_action_2_triggered()
{
//    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
//    pLayer->Clear();
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
