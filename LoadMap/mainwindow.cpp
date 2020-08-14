#include <QDebug>
#include <Map/Plot/IPersonInfo.h>
#include <Map/Plot/IPoint.h>
#include <Map/Plot/ILine.h>
#include <Map/Plot/IPolygon.h>
#include <Map/Plot/IModel.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SceneGraph/ISceneGraph.h"
#include "Map/IMap.h"
#include "Map/IPlotManager.h"
#include "Map/IPlotLayer.h"

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
    /// 绘制人
    m_pPersonInfo = dynamic_cast<IPersonInfo*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPersonInfo"));
    m_pPersonInfo->SetName("123");
    ScenePos pos;
    pos.fLon = 120;
    pos.fLat = 24;
    pos.bIsGeo = true;
    m_pPersonInfo->SetPos(pos);
    pLayer->AddSceneNode(m_pPersonInfo);

    /// 绘制点
    m_pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPoint"));
    pos.fLon = 121;
    pos.fLat = 25;
    pos.bIsGeo = true;
    m_pPoint->SetPos(pos);
    SceneColor color;
    color.fR = 1.0f;
    m_pPoint->SetPointSize(20.f);
    m_pPoint->SetColor(color);
    pLayer->AddSceneNode(m_pPoint);

    /// 绘制线
    m_pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("ILine"));
    m_pLine->AddPoint(0,pos);
    pos.fLon = 122;
    pos.fLat = 26;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 27;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    m_pLine->SetColor(color);
    pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPolygon"));
    m_pPolygon->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 26;
    m_pPolygon->AddPoint(1,pos);
    pos.fLon = 121;
    pos.fLat = 26.5;
    pos.bIsGeo = true;
    m_pPolygon->AddPoint(2,pos);
    m_pPolygon->SetColor(color);
    pLayer->AddSceneNode(m_pPolygon);

    auto m_pModel = dynamic_cast<IModel*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IModel"));
    m_pModel->Set2DAndSamllPic("ico/red.png",0);
    m_pModel->SetModelPath("model/AirPlane.ive");
    m_pModel->SetScalBit(50);
    m_pModel->SetName("王有情");

    m_pModel->SetPos(pos);
    pos.fLon = 123;
    pos.fLat = 26;
    m_pModel->SetPos(pos);
    pLayer->AddSceneNode(m_pModel);
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
