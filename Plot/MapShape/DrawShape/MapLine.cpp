#include "MapLine.h"

void CMapLine::InitNode()
{
    ImplMapSceneGeometry<IMapLine>::InitNode();

    m_pDrawLine = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    AddNode(m_pDrapeNode.get(),m_pDrawLine->AsOsgSceneNode()->GetOsgNode());
    SetGeometry(m_pDrawLine);
}
