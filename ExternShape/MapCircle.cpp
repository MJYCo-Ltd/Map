#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapCircle.h>
#include <VersionMathCommon.h>
#include <GisMath/GisMath.h>
CMapCircle::CMapCircle(ISceneGraph* pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
    InitNode();
}
void CMapCircle::SetCenter(const MapGeoPos& pos)
{
    if(pos != m_stCenter)
    {
        m_stCenter = pos;
        UpdateNode();
    }
}
void CMapCircle::SetRadius(const float& radius)
{
    if(m_fRadius != radius)
    {
       m_fRadius = radius;
       UpdateNode();
    }
}
void CMapCircle::SetColor(const SceneColor& color)
{
    m_pPolygon->SetPolygonColor(color);
}
void CMapCircle::SetFineness(int fineness)
{
    if(m_nFineness != fineness)
    {
        m_nFineness = fineness;
        UpdateNode();
    }
}
void CMapCircle::InitNode()
{
    m_pPolygon = dynamic_cast<IMapPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
}
void CMapCircle::UpdateNode()
{
    if(m_fRadius <0.01)
        return;
    double dTmpOffset = D2PI/m_nFineness;
    double dLon, dLat,dHeight;
    MapGeoPos Pos;
    std::vector<MapGeoPos> GeoMuPos;
    for(int i =0 ; i<= m_nFineness;i++)
    {
        GisMath::GeoCalEndGeo(m_stCenter.fLon*DD2R,m_stCenter.fLat*DD2R,0,i*dTmpOffset, 0, m_fRadius,dLon, dLat,dHeight);
        Pos.fLat = dLat*DR2D;
        Pos.fLon = dLon*DR2D;
        Pos.fHeight = dHeight;
        GeoMuPos.push_back(Pos);
    }
    m_pPolygon->SetMultPos(GeoMuPos);
}
