#include "MapPolygon3D.h"

void CMapPolygon3D::InitNode()
{
    ImplMapSceneGeometry<IMapPolygon3D>::InitNode();

    m_pDrawPolygon = dynamic_cast<IPolygon3D*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPolygon3D"));
    if(nullptr != m_pDrawPolygon)
    {
        SetGeometry(m_pDrawPolygon);
    }
}
