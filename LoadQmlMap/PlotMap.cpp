#include <QDebug>
#include <SceneGraph/ISceneGraph.h>
#include <Plot/Map/IMap.h>
#include <Plot/IPlot.h>
#include <Plot/Map/IMapPoint.h>
#include <Plot/Map/IMapLine.h>
#include <Plot/Map/IMapLayer.h>
#include <Plot/Map/IMapPolygon.h>
#include <SceneGraph/IWindow.h>

#include "DealWindowsMessage.h"
#include "PlotMap.h"

CPlotMap::CPlotMap()
{
    m_pDealWindowMessage = new CDealWindowsMessage;
}

void CPlotMap::SetSceneGraph(ISceneGraph *pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
    PlotLine();

    m_pSceneGraph->GetMainWindow()->SubMessage(m_pDealWindowMessage);
}

void CPlotMap::PlotLine()
{
    auto pLayer = m_pSceneGraph->GetMap()->CreateLayer("test");
    /// 绘制人
//    auto m_pPersonInfo = dynamic_cast<IPersonInfo*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPersonInfo"));
//    m_pPersonInfo->SetName("123");
    ScenePos pos;
//    pos.fLon = 120;
//    pos.fLat = 24;
//    pos.fHeight = 100;

//    m_pPersonInfo->SetPos(pos);
//    pLayer->AddSceneNode(m_pPersonInfo);

    /// 绘制点
    auto m_pPoint = dynamic_cast<IMapPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    pos.fX = 121;
    pos.fY = 25;
    pos.fZ = 0;
    m_pPoint->GetDrawPoint()->AddPoint(0,pos);
    SceneColor color;
    color.fG = .0f;
    color.fB = .0f;
    m_pPoint->GetDrawPoint()->SetColor(color);
    pLayer->AddSceneNode(m_pPoint);

    /// 绘制线
    auto m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    m_pLine->GetDrawLine()->AddPoint(0,pos);
    pos.fX = 122;
    pos.fY = 26;
    m_pLine->GetDrawLine()->AddPoint(0,pos);
    pos.fX = 123;
    pos.fY = 27;
    m_pLine->GetDrawLine()->AddPoint(0,pos);
    m_pLine->GetDrawLine()->SetColor(color);
    m_pLine->GetDrawLine()->SetLineWidth(10.f);
    pLayer->AddSceneNode(m_pLine);

    /// 绘制区域
    auto m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
    m_pPolygon->GetDrawPolygon()->AddPoint(0,pos);
    pos.fX = 123;
    pos.fY = 26;
    m_pPolygon->GetDrawPolygon()->AddPoint(1,pos);
    pos.fX = 121;
    pos.fY = 26.5;
    m_pPolygon->GetDrawPolygon()->AddPoint(2,pos);
    m_pPolygon->GetDrawPolygon()->SetColor(color);
    pLayer->AddSceneNode(m_pPolygon);
}

void CPlotMap::ClearLayer()
{
}
