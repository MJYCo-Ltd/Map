#ifndef H_HEADER_MAP_POINT_H
#define H_HEADER_MAP_POINT_H

#include <Plot/MapShape/IMapPoints.h>
#include <osgEarth/FeatureNode>
#include <Inner/ImplMapSceneNode.hpp>
class PointCallBack;

class CMapPoints:public ImplMapSceneNode<IMapPoints>
{
    friend class PointCallBack;
public:
    CONSTRUCTOR(CMapPoints,ImplMapSceneNode<IMapPoints>)

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

    void PointSizeChanged();
    void ColorChanged();
protected:
    float  m_fPointSize=10.0f;
    bool   m_bUpdate=false;
    SceneColor m_stColor;
    osg::observer_ptr<PointCallBack> m_pPointCallBack;
    osg::observer_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // H_HEADER_MAP_POINT_H
