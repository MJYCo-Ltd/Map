#ifndef CMAP_LAYER_H
#define CMAP_LAYER_H
#include <map>
#include <string>
#include <osg/Group>
#include <osgEarth/MapNode>
#include <Map/IMapLayer.h>

class ISceneGraph;
class CMapModelLayer;

using namespace std;
class CMapLayer : public IMapLayer
{
    friend class MapLayerAddNode;

public:
    CMapLayer(const string& sLayerName,osgEarth::MapNode* pMapNode,ISceneGraph* pSceneGraph);
    ~CMapLayer();

    /**
     * @brief 获取图层名字
     * @return
     */
    const string& GetLayerName(){return(m_sLayerName);}

    /**
     * @brief 添加节点
     */
    virtual unsigned int AddSceneNode(IMapSceneNode*pSceneNode);

    /**
     * @brief 移除节点
     */
    virtual void RemoveSceneNode(IMapSceneNode* pSceneNode);

    /**
     * @brief 清空所有的节点
     */
    virtual void Clear();

    /**
     * @brief 控制图层显隐
     */
    virtual void SetVisible(bool bVisible);

    /**
     * @brief 图层是否可见
     * @return
     */
    virtual bool IsVisible(){return(m_bVisible);}

    /**
     * @brief 更新地图
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 获取模型
     * @return
     */
    CMapModelLayer* GetModelLayer(){return(m_pModelLayer.get());}
protected:
    /**
     * @brief 移除节点
     */
    void RemoveNode(IMapSceneNode*);
private:
    ISceneGraph*                      m_pSceneGraph=nullptr;
    osg::observer_ptr<osgEarth::MapNode> m_pMapNode;
    osg::ref_ptr<CMapModelLayer> m_pModelLayer;
    string       m_sLayerName;
    unsigned int m_unID=0;
    bool         m_bVisible=true;
    map<unsigned int,IMapSceneNode*> m_mapID2Node;
};

#endif // CPLOTLAYER_H
