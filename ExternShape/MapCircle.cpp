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
void CMapCircle::SetCenter(const ScenePos &pos)
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
    m_pPolygon->GetDrawPolygon()->SetColor(color);
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
    {
        return;
    }
    double dTmpOffset = D2PI/m_nFineness;
    ScenePos Pos;
    std::vector<ScenePos> GeoMuPos;
    for(int i =0 ; i<= m_nFineness;i++)
    {
        GisMath::GeoCalEndGeo(m_stCenter.dX*DD2R,m_stCenter.dY*DD2R,0,i*dTmpOffset, 0,
                              m_fRadius,Pos.dX, Pos.dY,Pos.dZ);
        Pos.dX *= DR2D;
        Pos.dY *= DR2D;
        GeoMuPos.push_back(Pos);
    }
    m_pPolygon->GetDrawPolygon()->SetMultPos(GeoMuPos);
}
