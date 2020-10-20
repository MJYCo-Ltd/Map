#include "MapModifyLayer.h"

CMapModifyLayer::CMapModifyLayer(osgEarth::MapNode *pMapNode, osgEarth::Layer *pLayer, bool bAdd):
    m_pMapNode(pMapNode),
    m_pLayer(pLayer),
    m_bAdd(bAdd)
{
}

void CMapModifyLayer::operator()(osg::Object*)
{
    if(m_bAdd)
    {
        m_pMapNode->getMap()->addLayer(m_pLayer);
    }
    else
    {
        m_pMapNode->getMap()->removeLayer(m_pLayer);
    }
}
