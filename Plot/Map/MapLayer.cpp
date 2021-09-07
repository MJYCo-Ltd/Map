#include <Plot/Map/IMapSceneNode.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/OsgExtern/IOsgMapSceneNode.h>
#include <Inner/IOsgSceneNode.h>
#include "MapModelLayer.h"

#include "MapLayer.h"

/// 标绘图层
CMapLayer::CMapLayer(const std::string &sLayerName, ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph),
    m_sLayerName(sLayerName)
{
    m_pModelLayer = new CMapModelLayer(m_pSceneGraph);
    m_pModelLayer->Init();
}

/// 析构
CMapLayer::~CMapLayer()
{
    CMapLayer::Clear();
}

/// 添加到图层上
unsigned int CMapLayer::AddSceneNode(IMapSceneNode *pSceneNode)
{
    for(auto find = m_mapID2Node.begin();
        m_mapID2Node.end() != find; ++find)
    {
        if(pSceneNode == find->second)
        {
            return(find->first);
        }
    }

    m_mapID2Node[++m_unID] = pSceneNode;


    pSceneNode->AsOsgSceneNode()->AddToGroup(m_pModelLayer->getGroup());
    /// 添加到场景中去
    m_pModelLayer->AddSceneNode(pSceneNode);

    return(m_unID);
}

/// 移除节点
void CMapLayer::RemoveSceneNode(IMapSceneNode *pSceneNode)
{
    for(auto find = m_mapID2Node.begin();
        m_mapID2Node.end() != find; ++find)
    {
        if(pSceneNode == find->second)
        {
            /// 从场景中移除
            RemoveNode(pSceneNode);
            m_mapID2Node.erase(find);
            return;
        }
    }
}

/// 清空所有节点
void CMapLayer::Clear()
{
    for(auto find = m_mapID2Node.begin();
        m_mapID2Node.end() != find;)
    {
        /// 从场景中移除
        RemoveNode(find->second);
        find = m_mapID2Node.erase(find);
    }
}

/// 设置是否可见
void CMapLayer::SetVisible(bool bVisible)
{
    if(bVisible == m_bVisible)
    {
        return;
    }
    else
    {
        m_bVisible = bVisible;
        m_pModelLayer->setVisible(m_bVisible);
    }
}


/// 更新地图
void CMapLayer::UpdateMapNode()
{
    for(auto one : m_mapID2Node)
    {
        dynamic_cast<IOsgMapSceneNode*>(one.second)->UpdateMapNode();
    }
}

void CMapLayer::SetRenderOrder(int nOrder)
{
    m_pModelLayer->getGroup()->getOrCreateStateSet()->setRenderBinDetails(nOrder,"RenderBin");
}

/// 删除节点
void CMapLayer::RemoveNode(IMapSceneNode *pSceneNode)
{
    /// 从场景中移除
   m_pModelLayer->RemoveSceneNode(pSceneNode);
}
