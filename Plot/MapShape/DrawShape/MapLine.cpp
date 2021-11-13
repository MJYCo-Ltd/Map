#include "MapLine.h"

void CMapLine::InitNode()
{
    ImplMapSceneGeometry<IMapLine>::InitNode();

    m_pDrawLine = dynamic_cast<ILine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("ILine"));
    if(nullptr != m_pDrawLine)
    {
        SetGeometry(m_pDrawLine);
    }
}
