#include "MapPoint.h"

void CMapPoint::InitNode()
{
    ImplMapSceneGeometry<IMapPoint>::InitNode();

    m_pDrawPoint = dynamic_cast<IPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IPoint"));
    if(nullptr != m_pDrawPoint)
    {
        SetGeometry(m_pDrawPoint);
    }
}
