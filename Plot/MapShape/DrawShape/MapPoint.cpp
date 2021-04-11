#include "MapPoint.h"

void CMapPoint::InitNode()
{
    ImplMapSceneGeometry<IMapPoint>::InitNode();

    m_pDrawPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    AddNode(m_pDrapeNode.get(),m_pDrawPoint->AsOsgSceneNode()->GetOsgNode());
    SetGeometry(m_pDrawPoint);
}
