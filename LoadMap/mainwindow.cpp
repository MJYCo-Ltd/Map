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

int nTimers=0;
void MainWindow::timerEvent(QTimerEvent *event)
{
    ++nTimers;
    if(nTimers > 1)
    {
        on_action_2_triggered();
        nTimers = 0;
    }
    else
    {
        on_action_triggered();
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

    ScenePos pos;
    SceneColor color;
    color.fR=1.0f;

    /// 绘制线
    m_pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("ILine"));
    pos.fLon = 124.514;
    pos.fLat = 22.8686;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 121.69;
    pos.fLat = 20.3223;
    pos.bIsGeo = true;
    m_pLine->AddPoint(1,pos);
    pos.fLon = 118.276;
    pos.fLat = 19.3485;
    pos.bIsGeo = true;
    m_pLine->AddPoint(2,pos);
    pos.fLon = 116.44;
    pos.fLat = 16.863;
    pos.bIsGeo = true;
    m_pLine->AddPoint(3,pos);
    m_pLine->SetColor(color);
    pLayer->AddSceneNode(m_pLine);


    /// 绘制模型
    auto m_pModel = dynamic_cast<IModel*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IModel"));
    m_pModel->Set2DAndSamllPic("Images/ship.png",90);
    m_pModel->SetModelPath("model/AirPlane.ive");
    m_pModel->SetScalBit(50);
    m_pModel->SetName("王有情");
    pos.fLon=124.514;
    pos.fLat=22.8686;
    m_pModel->SetPos(pos);
    pos.fLon = 124.45060178914574;
    pos.fLat = 22.812955796767881;
    m_pModel->SetPos(pos);
    pLayer->AddSceneNode(m_pModel);


    /// 绘制脉冲波
    auto pPulse = dynamic_cast<IPulseSensor*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPulseSensor"));
    pPulse->SetPos(pos);
    pos.fHeight = 1e7;
    pPulse->SetEndPos(pos);
    pPulse->SetStartRadius(10000);
    pPulse->SetEndRadius(1e5);
    pPulse->SetPulseCount(50);
    pPulse->SetColor(color);
    pLayer->AddSceneNode(pPulse);
}

void MainWindow::on_action_2_triggered()
{
    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    pLayer->Clear();
}

void MainWindow::on_action_3_triggered()
{
    nTimerID = startTimer(1000);
    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    m_pPersonInfo = dynamic_cast<IPersonInfo*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPersonInfo"));
    pLayer->AddSceneNode(m_pPersonInfo);
}

void MainWindow::on_action_4_triggered()
{
    if(-1 != nTimerID)
    {
        killTimer(nTimerID);
    }

    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    pLayer->RemoveSceneNode(m_pPersonInfo);
}
