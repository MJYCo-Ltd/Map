#include <QDebug>
#include <Map/Plot/IPersonInfo.h>
#include <Map/Plot/IPoint.h>
#include <Map/Plot/ILine.h>
#include <Map/Plot/IPolygon.h>
#include <Map/Plot/IModel.h>
#include <Map/Plot/IConeSensor.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SceneGraph/ISceneGraph.h"
#include "Map/IMap.h"
#include "Map/IPlotManager.h"
#include "Map/IPlotLayer.h"
#include "SpaceEnv/ISpaceEnv.h"
#include "SpaceEnv/ISpaceBackGround.h"
#include "Map/Plot/ISConeSensor.h"
#include "Map/Plot/IPulseSensor.h"
#include "GisMath/GisMath.h"
#include <Sofa/sofam.h>

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
    ScenePos pos;
    pos.bIsGeo = true;
    pos.fHeight = 500;
    ++m_nTimes;
    double dLon,dLat;
    if(m_nTimes < 2000)
    {
        GisMath::CalBaiser(121.225100*DD2R,23.128880*DD2R,dA1,m_nTimes*dL1,dLon,dLat);
        pos.fLon = dLon*DR2D;
        pos.fLat = dLat*DR2D;
        m_pModel->SetPos(pos);

        m_pModel->SetYPR(dA1*DR2D,0,0);
    }
    else if(m_nTimes < 4000)
    {
        GisMath::CalBaiser(121.185947*DD2R,23.123019*DD2R,dA2,(m_nTimes-2000)*dL2,dLon,dLat);
        pos.fLon = dLon*DR2D;
        pos.fLat = dLat*DR2D;
        m_pModel->SetPos(pos);
        m_pModel->SetYPR(dA2*DR2D,0,0);
    }
    else if(m_nTimes < 6000)
    {
        GisMath::CalBaiser(121.178775*DD2R,23.101700*DD2R,dA3,(m_nTimes-4000)*dL3,dLon,dLat);
        pos.fLon = dLon*DR2D;
        pos.fLat = dLat*DR2D;
        m_pModel->SetPos(pos);
        m_pModel->SetYPR(dA3*DR2D,0,0);
    }
    else
    {
        killTimer(nTimerID);
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

void MainWindow::on_action_triggered()
{
    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    double dAizm2;
    GisMath::CalBaiserF(121.225100*DD2R,23.128880*DD2R,121.185947*DD2R,23.123019*DD2R,dA1,dAizm2,dL1);
    dL1 /= 2000.;
    GisMath::CalBaiserF(121.185947*DD2R,23.123019*DD2R,121.178775*DD2R,23.101700*DD2R,dA2,dAizm2,dL2);
    dL2 /= 2000.;
    GisMath::CalBaiserF(121.178775*DD2R,23.101700*DD2R,121.164617*DD2R,23.059267*DD2R,dA3,dAizm2,dL3);
    dL3 /= 2000.;

    ScenePos pos;
    SceneColor color;
    color.fR=1.0f;

    /// 添加线
    m_pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("ILine"));
    pos.fLon = 121.225100;
    pos.fLat = 23.128880;
    pos.fHeight = 500;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 121.185947;
    pos.fLat = 23.123019;
    pos.bIsGeo = true;
    m_pLine->AddPoint(1,pos);
    pos.fLon = 121.178775;
    pos.fLat = 23.101700;
    pos.bIsGeo = true;
    m_pLine->AddPoint(2,pos);
    pos.fLon = 121.164617;
    pos.fLat = 23.059267;
    pos.bIsGeo = true;
    m_pLine->AddPoint(3,pos);
    m_pLine->SetColor(color);
    //m_pLine->SetTerrainType(RELATIVE_TERRAIN);
    pLayer->AddSceneNode(m_pLine);


    /// 添加模型
    m_pModel = dynamic_cast<IModel*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IModel"));
    m_pModel->Set2DAndSamllPic("Images/ship.png",90);
    m_pModel->SetModelPath("model/AirPlane.ive");
    //m_pModel->SetScalBit(1);
    m_pModel->SetName("李晓强");
    pos.fLon=121.225100;
    pos.fLat=23.128880;
    m_pModel->SetPos(pos);
    pLayer->AddSceneNode(m_pModel);

    m_pSceneGraph->GetMainWindow()->GetMainViewPoint()->SetTrackNode(m_pModel);
}

void MainWindow::on_action_2_triggered()
{
    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    pLayer->Clear();
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
