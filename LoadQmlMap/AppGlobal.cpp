#include <QGuiApplication>
#include <QDebug>
#include <ISceneCore.h>
#include <SceneGraph/ISceneGraphManager.h>
#include "AppGlobal.h"
#include "PlotMap.h"
#include "../ScenarioManager/ScenarioManager.h"
#include "../AreaPlanManager/AreaPlanManager.h"

QAppGlobal::QAppGlobal(QObject *parent) : QObject(parent)
{
    m_pScenarioManager = new ScenarioManager;
    m_pAreaPlanManager = new AreaPlanManager;
    m_pPlotMap = new CPlotMap;
    setObjectName("AppGlobal");
}

QAppGlobal::~QAppGlobal()
{
    delete m_pPlotMap;
    delete m_pAreaPlanManager;
    delete m_pScenarioManager;
}

/// 设置osgItem
void QAppGlobal::setOsgItem(QQuickItem *pOsgItem)
{
    m_pOsgItem = pOsgItem;
    auto pSeneGraph = GetSceneCore()->GetSceneGraphManager()->FindSceneGraph(pOsgItem);
    m_pPlotMap->SetSceneGraph(pSeneGraph);
    m_pScenarioManager->setSceneGraph(pSeneGraph);
    m_pAreaPlanManager->setSceneGraph(pSeneGraph);
}

void QAppGlobal::plotALine()
{
    m_pPlotMap->PlotLine();
}

void QAppGlobal::clearPlot()
{
    m_pPlotMap->ClearLayer();
}

void QAppGlobal::init()
{
    m_pScenarioManager->setDir(QCoreApplication::applicationDirPath() + "/Data/Scenarios");
    if (m_pScenarioManager->scenarioList().count() == 0)
        qDebug() << "NO SCENARIO!";
    m_pScenarioManager->addItem(m_pAreaPlanManager);
    //m_pAreaPlanManager->load(QGuiApplication::applicationDirPath() + "/Data/AreaPlan");
}

ScenarioManager* QAppGlobal::scenarioManager()
{
    //qDebug() << "QAppGlobal::scenarioManager() :" << m_pScenarioManager;
    return m_pScenarioManager;
}

AreaPlanManager* QAppGlobal::areaPlanManager()
{
    qDebug() << "QAppGlobal::areaPlanManager() :" << m_pAreaPlanManager;
    return m_pAreaPlanManager;
}
