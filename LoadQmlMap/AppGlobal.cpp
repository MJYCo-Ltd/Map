#include <QGuiApplication>
#include <QDebug>
#include <ISceneCore.h>
#include <SceneGraph/ISceneGraphManager.h>
#include "AppGlobal.h"
#include "PlotMap.h"
#include "../AreaPlanManager/AreaPlanManager.h"

QAppGlobal::QAppGlobal(QObject *parent) : QObject(parent)
{
    m_pAreaPlanManager = new AreaPlanManager;
    m_pPlotMap = new CPlotMap;
    setObjectName("AppGlobal");
}

QAppGlobal::~QAppGlobal()
{
    delete m_pPlotMap;
}

/// 设置osgItem
void QAppGlobal::setOsgItem(QQuickItem *pOsgItem)
{
    m_pOsgItem = pOsgItem;
    auto pSeneGraph = GetSceneCore()->GetSceneGraphManager()->FindSceneGraph(pOsgItem);
    m_pPlotMap->SetSceneGraph(pSeneGraph);
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
    m_pAreaPlanManager->load(QGuiApplication::applicationDirPath() + "/Data/AreaPlan");
}
