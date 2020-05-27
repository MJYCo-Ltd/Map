#include <algorithm>

#include <osg/OperationThread>
#include <osgEarth/Version>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>

#include "MapNodeFactory.h"
#include "PlotLayer.h"
#include "PlotManager.h"
#include <QDebug>
class ChangeMapNode:public osg::Operation
{
public:
    ChangeMapNode(osgEarth::MapNode* pOldMapNode,
                  osgEarth::MapNode* pNewMapNode,
                  osgEarth::ModelLayer* pModelLayer,
		          osg::Group* pModelNode,
                  CPlotManager* pPlotManager):
        m_pOld(pOldMapNode),
        m_pNew(pNewMapNode),
        m_pModelLayer(pModelLayer),
		m_pModelNode(pModelNode),
        m_pPlotManager(pPlotManager){}


    void operator()(osg::Object*)
    {
        if(m_pOld.valid())
        {
            m_pOld->getMap()->removeLayer(m_pModelLayer);

			if (0 == m_pModelLayer->getNode()->asGroup()->getNumChildren())
			{
				m_pModelLayer->setNode(m_pModelNode);
			}
        }

		m_pNew->getMap()->addLayer(m_pModelLayer);

         
        /// 更新
        for(auto one : m_pPlotManager->m_mapLayer)
        {
            one.second->UpdateMapNode(m_pNew);
        }

    }
private:
    osg::ref_ptr<osgEarth::MapNode> m_pOld; /// 旧的地图节点
    osg::ref_ptr<osgEarth::MapNode> m_pNew; /// 新的地图节点
    osg::ref_ptr<osgEarth::ModelLayer> m_pModelLayer; /// 模型图层
	osg::ref_ptr<osg::Group> m_pModelNode;
    CPlotManager* m_pPlotManager;
};


CPlotManager::CPlotManager(ISceneGraph* pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    m_pModelNode = new osg::Group;
#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
    m_pModelLayer = new osgEarth::ModelLayer;
    m_pModelLayer->setNode(m_pModelNode);
#else
    m_pModelLayer = new osgEarth::ModelLayer("PlotModel",m_pModelNode);
#endif

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
                    new CModifyNode(m_pModelNode,one->GetOsgNode(),true));
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
                new ChangeMapNode(pOldMapNode,pNewMapNode,m_pModelLayer,m_pModelNode.get(), this));
}

/// 根据接口类型创建地图节点
IMapSceneNode *CPlotManager::CreateMapSceneNode(const string& csInterfaceName)
{
    return(m_pCreateFactory->CreateMapSceneNode(csInterfaceName));
}

bool CPlotManager::DeleteMapSceneNode(IMapSceneNode *pMapSceneNode)
{
    return(m_pCreateFactory->DeleteMapSceneNode(pMapSceneNode));
}
