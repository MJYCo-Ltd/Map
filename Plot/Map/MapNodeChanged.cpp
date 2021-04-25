#include "MapNodeChanged.h"
#include "Map.h"
#include "MapLayer.h"
#include "MapModelLayer.h"

CMapNodeChanged::CMapNodeChanged(osgEarth::MapNode* pOldMapNode,osgEarth::MapNode* pNewMapNode,CMap* pMap):
    m_pOldMapNode(pOldMapNode),
    m_pNewMapNode(pNewMapNode),
    m_pMap(pMap)
{
}

void CMapNodeChanged::operator()(osg::Object *)
{
    if(m_pOldMapNode.valid())
    {
        for(auto one=m_pMap->m_userLayers.begin();one != m_pMap->m_userLayers.end();++one)
        {
            m_pOldMapNode->getMap()->removeLayer(one->second->GetModelLayer());
        }

    }

    /// 如果新的地图节点有效
    if(m_pNewMapNode.valid())
    {
        m_pNewMapNode->getMap()->beginUpdate();
        for(auto one=m_pMap->m_userLayers.begin();one != m_pMap->m_userLayers.end();++one)
        {
            m_pNewMapNode->getMap()->addLayer(one->second->GetModelLayer());
            one->second->UpdateMapNode();
        }
        m_pNewMapNode->getMap()->endUpdate();
    }
}
