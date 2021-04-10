#ifndef H_HEADER_MAP_POINT_H
#define H_HEADER_MAP_POINT_H

#include <Plot/Map/IMapPoint.h>
#include <osgEarth/FeatureNode>
#include <Inner/ImplMapShape.hpp>

class CMapPoint:public ImplMapShape<IMapPoint>
{
public:
    CONSTRUCTOR(CMapPoint,ImplMapShape<IMapPoint>)

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
     * @brief 位置更新消息
     */
    void PosChanged();

    /**
     * @brief 初始化样式
     */
    void InitStyle();

    void UpdateNode();
    void PointSizeChanged();
    void ColorChanged();
protected:
    bool   m_bPosChanged=false;
    osg::observer_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // H_HEADER_MAP_POINT_H
