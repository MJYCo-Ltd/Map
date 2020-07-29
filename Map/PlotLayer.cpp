#include <Map/IMapSceneNode.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IOsgEarthMapSceneNode.h>
#include <Inner/IOsgSceneNode.h>

#include "PlotLayer.h"


class PlotLayerAddNode:public osg::Operation
{
public:
    PlotLayerAddNode(CPlotLayer* pLayer,IOsgEarthMapSceneNode* pMapSceneNode):
        m_pLayer(pLayer),
        m_pMapSceneNode(pMapSceneNode){}

    void operator () (osg::Object*)
    {
        m_pMapSceneNode->UpdateMapNode(m_pLayer->m_pMapNode.get());
        m_pLayer->m_pGroup->addChild(dynamic_cast<IOsgSceneNode*>(m_pMapSceneNode)->GetOsgNode());
    }
private:
    CPlotLayer* m_pLayer;
    IOsgEarthMapSceneNode* m_pMapSceneNode;
};

/// 标绘图层
CPlotLayer::CPlotLayer(const string &sLayerName, osgEarth::MapNode *pMapNode, ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph),
    m_pMapNode(pMapNode),
    m_sLayerName(sLayerName)
{
    m_pGroup = new osg::Group;
}

/// 析构
CPlotLayer::~CPlotLayer()
{
    Clear();
}

/// 添加到图层上
unsigned int CPlotLayer::AddSceneNode(IMapSceneNode *pSceneNode)
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


    /// 添加到场景中去
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new PlotLayerAddNode(this,dynamic_cast<IOsgEarthMapSceneNode*>(pSceneNode)));

    return(m_unID);
}

/// 移除节点
void CPlotLayer::RemoveSceneNode(IMapSceneNode *pSceneNode)
{
    for(auto find = m_mapID2Node.begin();
        m_mapID2Node.end() != find; ++find)
    {
        if(pSceneNode == find->second)
        {
            /// 从场景中移除
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(dynamic_cast<IOsgSceneNode*>(m_pSceneGraph->GetRoot())->GetOsgNode(),
                                                                                  dynamic_cast<IOsgSceneNode*>(pSceneNode)->GetOsgNode(),false));
            m_mapID2Node.erase(find);
            return;
        }
    }
}

void CPlotLayer::Clear()
{
    for(auto find = m_mapID2Node.begin();
        m_mapID2Node.end() != find;)
    {
        /// 从场景中移除
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyNode(m_pGroup,dynamic_cast<IOsgSceneNode*>(find->second)->GetOsgNode(),false));
        find = m_mapID2Node.erase(find);
    }
}

/// 设置是否可见
void CPlotLayer::SetVisible(bool bVisible)
{
    if(bVisible == m_bVisible)
    {
        return;
    }
    else
    {
        m_bVisible = bVisible;
        if(m_bVisible)
        {
            m_pGroup->setNodeMask(-1);
        }
        else
        {
            m_pGroup->setNodeMask(0);
        }
    }
}

/// 更新地图
void CPlotLayer::UpdateMapNode(osgEarth::MapNode *pMapNode)
{
    for(auto one : m_mapID2Node)
    {
        dynamic_cast<IOsgEarthMapSceneNode*>(one.second)->UpdateMapNode(pMapNode);
    }
}

/// 获取渲染节点
osg::Node *CPlotLayer::GetOsgNode()
{
    return (m_pGroup);
}
