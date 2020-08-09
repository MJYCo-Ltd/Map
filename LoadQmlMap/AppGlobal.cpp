#include <QDebug>
#include <ISceneCore.h>
#include <SceneGraph/ISceneGraphManager.h>
#include "AppGlobal.h"
#include "PlotMap.h"

QAppGlobal::QAppGlobal(QObject *parent) : QObject(parent)
{
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
}

void QAppGlobal::plotALine()
{
    m_pPlotMap->PlotLine();
}

void QAppGlobal::clearPlot()
{
    m_pPlotMap->ClearLayer();
}
