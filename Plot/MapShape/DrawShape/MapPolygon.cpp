#include "MapPolygon.h"

void CMapPolygon::InitNode()
{
    ImplMapSceneGeometry<IMapPolygon>::InitNode();

    m_pDrawPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon"));
    AddNode(m_pDrapeNode.get(),m_pDrawPolygon->AsOsgSceneNode()->GetOsgNode());
    SetGeometry(m_pDrawPolygon);
}
