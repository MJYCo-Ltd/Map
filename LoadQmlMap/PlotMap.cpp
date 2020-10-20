#include <QDebug>
#include <SceneGraph/ISceneGraph.h>
#include <Map/IMap.h>

#include "PlotMap.h"

CPlotMap::CPlotMap()
{

}

void CPlotMap::SetSceneGraph(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
}

void CPlotMap::PlotLine()
{
//    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
//    /// 绘制人
//    auto m_pPersonInfo = dynamic_cast<IPersonInfo*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPersonInfo"));
//    m_pPersonInfo->SetName("123");
//    ScenePos pos;
//    pos.fLon = 120;
//    pos.fLat = 24;
//    pos.fHeight = 100;
//    pos.bIsGeo = true;
//    m_pPersonInfo->SetPos(pos);
//    pLayer->AddSceneNode(m_pPersonInfo);

//    /// 绘制点
//    auto m_pPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPoint"));
//    pos.fLon = 121;
//    pos.fLat = 25;
//    pos.fHeight = 1000;
//    pos.bIsGeo = true;
//    m_pPoint->SetPos(pos);
//    SceneColor color;
//    color.fR = 1.0f;
//    m_pPoint->SetColor(color);
//    pLayer->AddSceneNode(m_pPoint);

//    /// 绘制线
//    auto m_pLine = dynamic_cast<ILine*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("ILine"));
//    m_pLine->AddPoint(0,pos);
//    pos.fLon = 122;
//    pos.fLat = 26;
//    pos.fHeight = 1000;
//    pos.bIsGeo = true;
//    m_pLine->AddPoint(0,pos);
//    pos.fLon = 123;
//    pos.fLat = 27;
//    pos.fHeight = 1000;
//    pos.bIsGeo = true;
//    m_pLine->AddPoint(0,pos);
//    m_pLine->SetColor(color);
//    pLayer->AddSceneNode(m_pLine);

//    /// 绘制区域
//    auto m_pPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetMap()->GetPlotManager()->CreateMapSceneNode("IPolygon"));
//    m_pPolygon->AddPoint(0,pos);
//    pos.fLon = 123;
//    pos.fLat = 26;
//    pos.fHeight = 1000;
//    pos.bIsGeo = true;
//    m_pPolygon->AddPoint(1,pos);
//    pos.fLon = 121;
//    pos.fLat = 26.5;
//    pos.fHeight = 1000;
//    pos.bIsGeo = true;
//    m_pPolygon->AddPoint(2,pos);
//    m_pPolygon->SetColor(color);
//    pLayer->AddSceneNode(m_pPolygon);
}

void CPlotMap::ClearLayer()
{
//    auto pLayer = m_pSceneGraph->GetMap()->GetPlotManager()->FindOrCreateLayer("test");
//    pLayer->Clear();
}
