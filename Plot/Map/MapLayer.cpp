#include <Plot/Map/IMapSceneNode.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IOsgMapSceneNode.h>
#include <Inner/IOsgSceneNode.h>
#include "MapModelLayer.h"

#include "MapLayer.h"

class MapLayerAddNode:public osg::Operation
{
public:
    MapLayerAddNode(CMapLayer* pLayer,IOsgMapSceneNode* pMapSceneNode):
        m_pLayer(pLayer),
        m_pMapSceneNode(pMapSceneNode){}

    void operator()(osg::Object*)
    {
        m_pMapSceneNode->UpdateMapNode(m_pLayer->m_pMapNode.get());
        m_pLayer->m_pModelLayer->getGroup()->addChild(dynamic_cast<IOsgSceneNode*>(m_pMapSceneNode)->GetOsgNode());
    }
private:
    CMapLayer* m_pLayer;
    IOsgMapSceneNode* m_pMapSceneNode;
};

/// 标绘图层
CMapLayer::CMapLayer(const std::string &sLayerName, osgEarth::MapNode *pMapNode, ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph),
    m_pMapNode(pMapNode),
    m_sLayerName(sLayerName)
{
    m_pModelLayer = new CMapModelLayer;
}

/// 析构
CMapLayer::~CMapLayer()
{
    Clear();
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
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new MapLayerAddNode(this,dynamic_cast<IOsgMapSceneNode*>(pSceneNode)));

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
void CMapLayer::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    m_pMapNode = pMapNode;
    for(auto one : m_mapID2Node)
    {
        dynamic_cast<IOsgMapSceneNode*>(one.second)->UpdateMapNode(pMapNode);
    }
}

/// 删除节点
void CMapLayer::RemoveNode(IMapSceneNode *pSceneNode)
{
    pSceneNode->AsOsgSceneNode()->DelFromGroup(m_pModelLayer->getGroup());
    /// 从场景中移除
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                new CModifyNode(m_pModelLayer->getGroup(),pSceneNode->AsOsgSceneNode()->GetOsgNode(),false));
}
