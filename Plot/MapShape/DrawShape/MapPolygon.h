#ifndef CHEADER_SENEPOLYGON_H
#define CHEADER_SENEPOLYGON_H

#include <osgEarth/FeatureNode>
#include <Plot/Map/IMapPolygon.h>
#include <Inner/ImplMapShape.hpp>

class CMapPolygon:public ImplMapShape<IMapPolygon>
{
public:
    CONSTRUCTOR(CMapPolygon,ImplMapShape<IMapPolygon>)

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
    /**
     * @brief 多边形颜色更改
     */
    void PolygonColorChanged();

    /**
     * @brief 更新回调
     */
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
    bool  m_bPosChanged=false;
};

#endif // CSENEPOLYGON_H
