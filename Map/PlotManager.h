#ifndef CPLOTMANAGER_H
#define CPLOTMANAGER_H
#include<map>
using namespace std;

#include<osgEarth/MapNode>
#include <osgEarth/ModelLayer>

#include <Map/IPlotManager.h>
#include <Map/IPlotLayer.h>

class CPlotLayer;
class ISceneGraph;
class CMapNodeFactory;

class CPlotManager:public IPlotManager
{
    friend class ChangeMapNode;

public:
    CPlotManager(ISceneGraph* pSceneGraph);
    ~CPlotManager();

    /**
     * @brief 创建标绘图层
     * @return
     */
    IPlotLayer* FindOrCreateLayer(const string& sLayerName);

    /**
     * @brief 移除图层
     * @return
     */
    bool RemoveLayer(IPlotLayer*& pLayer);

    /**
     * @brief 移除图层
     * @return
     */
    bool RemoveLayer(const string& sLayerName);

    /**
     * @brief 清空所有图层
     */
    virtual void Clear();

    /**
     * @brief 更新地图
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pOldMapNode, osgEarth::MapNode *pNewMapNode);

    /**
     * @brief 创建地图节点
     * @param uType
     * @return
     */
    IMapSceneNode* CreateMapSceneNode(unsigned int uType);

    /**
     * @brief 删除地图节点
     * @return
     */
    bool DeleteMapSceneNode(IMapSceneNode* pMapSceneNode);
private:
    ISceneGraph*                       m_pSceneGraph; /// 场景图
    CMapNodeFactory*                   m_pCreateFactory;/// 创建节点
    map<string,CPlotLayer*>            m_mapLayer;    /// 所有的图层
    osg::observer_ptr<osgEarth::MapNode> m_pMapNode;
    osg::ref_ptr<osgEarth::ModelLayer> m_pModelLayer; /// 模型图层
    osg::ref_ptr<osg::Group>           m_pModelNode;  /// 模型图层对应的节点
};

#endif // CPLOTMANAGER_H
