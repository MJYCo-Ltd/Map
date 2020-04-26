#include <Map/Plot/IPersonInfo.h>
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
    m_pPersonInfo->Add();
//    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");

//    for(int i=0; i<1000;++i)
//    {
//        auto pNode = m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode(1);
//        pLayer->AddSceneNode(pNode);
//        m_listNode.push_back(pNode);
//    }
}

void MainWindow::on_action_2_triggered()
{
    m_pPersonInfo->Del();
//    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
//    pLayer->Clear();
//    for(auto one : m_listNode)
//    {
//        m_pSceneGraph->GetMap()->GetPlotManager()->DeleteMapSceneNode(one);
//    }
//    m_listNode.clear();
}

void MainWindow::on_action_3_triggered()
{
    nTimerID = startTimer(1000);
    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
    m_pPersonInfo = dynamic_cast<IPersonInfo*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode(1));
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
    m_pSceneGraph->GetMap()->GetPlotManager()->DeleteMapSceneNode(m_pPersonInfo);
}
