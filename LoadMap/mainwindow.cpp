#include <QDebug>
#include <Map/Plot/IPersonInfo.h>
#include <Map/Plot/IPoint.h>
#include <Map/Plot/ILine.h>
#include <Map/Plot/IPolygon.h>
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
    pos.fHeight = 100;
    pos.bIsGeo = true;
    m_pPersonInfo->SetPos(pos);
    pLayer->AddSceneNode(m_pPersonInfo);

    /// 绘制点
    m_pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPoint"));
    pos.fLon = 121;
    pos.fLat = 25;
    pos.fHeight = 1000;
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
    pos.fHeight = 1000;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 27;
    pos.fHeight = 1000;
    pos.bIsGeo = true;
    m_pLine->AddPoint(0,pos);
    m_pLine->SetColor(color);
    pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPolygon"));
    m_pPolygon->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 26;
    pos.fHeight = 1000;
    pos.bIsGeo = true;
    m_pPolygon->AddPoint(1,pos);
    pos.fLon = 121;
    pos.fLat = 26.5;
    pos.fHeight = 1000;
    pos.bIsGeo = true;
    m_pPolygon->AddPoint(2,pos);
    m_pPolygon->SetColor(color);
    pLayer->AddSceneNode(m_pPolygon);

    ///  坐标转换
    int nX,nY;
    if(m_pSceneGraph->GetMap()->ConvertCoord(nX,nY,pos,1))
    {
        qDebug()<<nX<<','<<nY<<'\n'<<pos.fLon<<','<<pos.fLat;
        if(m_pSceneGraph->GetMap()->ConvertCoord(nX,nY,pos,1))
        {
            qDebug()<<pos.fLon<<','<<pos.fLat;
        }
    }
    else
    {
        qDebug()<<"ConvertCoord falid";
    }
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
