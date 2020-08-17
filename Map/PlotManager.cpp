#include <algorithm>
#include <osg/OperationThread>
#include <osgEarth/Version>
#include <osgEarth/MapNode>
#include <osgEarth/AnnotationLayer>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "MyModelLayer.h"

#include "MapNodeFactory.h"
#include "PlotLayer.h"
#include "PlotManager.h"

class ChangeMapNode:public osg::Operation
{
public:
    ChangeMapNode(osgEarth::MapNode* pOldMapNode,
                  osgEarth::MapNode* pNewMapNode,
                  CMyModelLayer* pModelLayer,
                  CPlotManager* pPlotManager):
        m_pOld(pOldMapNode),
        m_pNew(pNewMapNode),
        m_pMyModelLayer(pModelLayer),
        m_pPlotManager(pPlotManager){}


    void operator()(osg::Object*)
    {
        if(m_pOld.valid())
        {
            m_pOld->getMap()->removeLayer(m_pMyModelLayer);
        }

        m_pNew->getMap()->addLayer(m_pMyModelLayer);

         
        /// 更新
        for(auto one : m_pPlotManager->m_mapLayer)
        {
            one.second->UpdateMapNode(m_pNew);
        }

    }
private:
    osg::ref_ptr<osgEarth::MapNode> m_pOld; /// 旧的地图节点
    osg::ref_ptr<osgEarth::MapNode> m_pNew; /// 新的地图节点
    osg::ref_ptr<CMyModelLayer> m_pMyModelLayer; /// 模型图层
    CPlotManager* m_pPlotManager=nullptr;
};


CPlotManager::CPlotManager(ISceneGraph* pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{

    m_pMyModelLayer = new CMyModelLayer;
    m_pCreateFactory = new CMapNodeFactory(m_pSceneGraph);
}

CPlotManager::~CPlotManager()
{
    Clear();
}

/// 查找或者创建图层
IPlotLayer *CPlotManager::FindOrCreateLayer(const string &sLayerName)
{
    auto findOne = m_mapLayer.find(sLayerName);
    if(m_mapLayer.end() == findOne)
    {
        auto one = new CPlotLayer(sLayerName,m_pMapNode.get(),m_pSceneGraph);
        m_mapLayer[sLayerName] = one;
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CModifyNode(m_pMyModelLayer->getGroup(),one->GetOsgNode(),true));
        return(one);
    }
    else
    {
        return findOne->second;
    }
}

/// 移除图层
bool CPlotManager::RemoveLayer(IPlotLayer *&pLayer)
{
    auto findLayer = m_mapLayer.begin();
    while(m_mapLayer.end() != findLayer)
    {
        if(pLayer == findLayer->second)
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                        new CModifyNode(m_pMyModelLayer->getGroup(),static_cast<CPlotLayer*>(pLayer)->GetOsgNode(),false));
            m_mapLayer.erase(findLayer);
            delete pLayer;
            pLayer = nullptr;
            return(true);
        }
        ++findLayer;
    }

    return(false);

}

/// 移除图层
bool CPlotManager::RemoveLayer(const string &sLayerName)
{
    auto findLayer = m_mapLayer.find(sLayerName);

    if(m_mapLayer.end() != findLayer)
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CModifyNode(m_pMyModelLayer->getGroup(),static_cast<CPlotLayer*>(findLayer->second)->GetOsgNode(),false));
        delete findLayer->second;
        m_mapLayer.erase(findLayer);
        return(true);
    }
    else
    {
        return(false);
    }
}

/// 清空所有图层
void CPlotManager::Clear()
{
    for(auto one : m_mapLayer)
    {
        delete one.second;
    }

    m_mapLayer.clear();
    delete m_pCreateFactory;
}

/// 更新MapNode
void CPlotManager::UpdateMapNode(osgEarth::MapNode *pOldMapNode,osgEarth::MapNode* pNewMapNode)
{
    m_pMapNode = pNewMapNode;
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                new ChangeMapNode(pOldMapNode,pNewMapNode,m_pMyModelLayer,this));
}

/// 根据接口类型创建地图节点
IMapSceneNode *CPlotManager::CreateMapSceneNode(const string& csInterfaceName)
{
    return(m_pCreateFactory->CreateMapSceneNode(csInterfaceName));
}

/// 强制删除没用的地图节点
void CPlotManager::FreeNoUseMapSceneNode()
{
    m_pCreateFactory->DeleteNoUseSceneNode();
}

/// 注册可以标绘的类型
void CPlotManager::RegisterPlotType()
{
    m_pCreateFactory->InitFactory();
}
