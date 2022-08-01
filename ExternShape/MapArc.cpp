#include <SceneGraph/ISceneGraph.h>
#include <Plot/IPlot.h>
#include <ExternShape/MapArc.h>
#include <VersionMathCommon.h>
#include <GisMath/GisMath.h>
CMapArc::CMapArc(ISceneGraph* pSceneGraph)
{
    m_pSceneGraph = pSceneGraph;
    InitNode();
}
void CMapArc::SetCenter(const ScenePos &pos)
{
    if(pos != m_stCenter)
    {
        m_stCenter = pos;
        UpdateNode();
    }
}
void CMapArc::SetRadius(const float& radius)
{
    if(m_fRadius != radius)
    {
       m_fRadius = radius;
       UpdateNode();
    }
}
void CMapArc::SetColor(const SceneColor& color)
{
    m_pMapLine->GetDrawLine()->SetColor(color);
}
void CMapArc::SetFineness(int fineness)
{
    if(m_nFineness != fineness)
    {
        m_nFineness = fineness;
        UpdateNode();
    }
}

void CMapArc::SetAngle(double dStartAngle, double dEndAngle)
{
    m_dStartAngle = dStartAngle;
    m_dEndAngle = dEndAngle;
}
void CMapArc::InitNode()
{
    m_pMapLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
}
void CMapArc::UpdateNode()
{
    if(m_fRadius <0.01)
    {
        return;
    }
    //过了360度的情况
    double dStartAngle = m_dStartAngle + (DPI / 2.0 * 3.0);
    dStartAngle = fmod(dStartAngle, D2PI);
    double dEndAngle = m_dEndAngle + (DPI / 2.0 * 3.0);
    dEndAngle = fmod(dEndAngle, D2PI);
    if(dEndAngle < dStartAngle)
        dEndAngle += D2PI;

    double dTmpOffset = D2PI/m_nFineness;
    ScenePos Pos;
    std::vector<ScenePos> GeoMuPos;
    double dTmpAngle = dStartAngle;
    while(true)
    {
        if(dTmpAngle > dEndAngle)
            dTmpAngle = dEndAngle;

        GisMath::GeoCalEndGeo(m_stCenter.dX*DD2R,m_stCenter.dY*DD2R,0, dTmpAngle > D2PI ? dTmpAngle - D2PI : dTmpAngle, 0,
                              m_fRadius,Pos.dX, Pos.dY,Pos.dZ);
        Pos.dX *= DR2D;
        Pos.dY *= DR2D;
        GeoMuPos.push_back(Pos);

        if(dTmpAngle == dEndAngle)
            break;
        dTmpAngle += dTmpOffset;
    }
    m_pMapLine->GetDrawLine()->SetMultPos(GeoMuPos);
}
