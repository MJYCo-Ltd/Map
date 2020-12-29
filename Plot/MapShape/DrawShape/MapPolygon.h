#ifndef CHEADER_SENEPOLYGON_H
#define CHEADER_SENEPOLYGON_H

#include <osgEarth/FeatureNode>
#include <Plot/Map/IMapPolygon.h>
#include <Inner/ImplMapSceneNode.hpp>

class CMapPolygon:public ImplMapSceneNode<IMapPolygon>
{
public:
    CONSTRUCTOR(CMapPolygon,ImplMapSceneNode<IMapPolygon>)

    /**
     * @brief 增加点
     */
    virtual void AddPoint(int, const MapGeoPos &);

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int);

    /**
     * @brief 更新指定位置点
     */
    virtual bool UpdatePoint(int,const MapGeoPos&);

    /**
     * @brief 设置多个位置
     */
    virtual void SetMultPos(const std::vector<MapGeoPos> &);

protected:
    void PolygonColorChanged();

    void UpdateNode();

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 初始化样式
     */
    void InitStyle();
private:
    osg::observer_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CSENEPOLYGON_H
