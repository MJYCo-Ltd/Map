#ifndef CMAP_LAYER_H
#define CMAP_LAYER_H
#include <map>
#include <string>
#include <osg/Group>
#include <osgEarth/MapNode>
#include <Plot/Map/IMapLayer.h>

class ISceneGraph;
class CMapModelLayer;

class CMapLayer : public IMapLayer
{
public:
    CMapLayer(const std::string& sLayerName, ISceneGraph* pSceneGraph);
    ~CMapLayer();

    /**
     * @brief 获取图层名字
     * @return
     */
    const std::string& GetLayerName(){return(m_sLayerName);}

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
    void UpdateMapNode();

    /**
     * @brief 获取模型
     * @return
     */
    CMapModelLayer* GetModelLayer(){return(m_pModelLayer.get());}

    /**
     * @brief 设置渲染顺序
     * @param nOrder
     */
    void SetRenderOrder(int nOrder);
protected:
    /**
     * @brief 移除节点
     */
    void RemoveNode(IMapSceneNode*);
private:
    ISceneGraph*                 m_pSceneGraph{};
    osg::ref_ptr<CMapModelLayer> m_pModelLayer;
    std::string                  m_sLayerName;
    unsigned int                 m_unID{};
    bool                         m_bVisible{true};
    std::map<unsigned int,IMapSceneNode*> m_mapID2Node;
};

#endif // CPLOTLAYER_H
