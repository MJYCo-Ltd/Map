#include "MapPolygon.h"

void CMapPolygon::InitNode()
{
    ImplMapSceneGeometry<IMapPolygon>::InitNode();

    m_pDrawPolygon = dynamic_cast<IPolygon*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon"));
    if(nullptr != m_pDrawPolygon)
    {
        SetGeometry(m_pDrawPolygon);
    }
}

void CMapPolygon::Update()
{
    UpdateMapNode();
}
