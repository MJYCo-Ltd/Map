#ifndef H_HEADER_CSCENELINE_H
#define H_HEADER_CSCENELINE_H

#include <osgEarth/FeatureNode>
#include <Plot/MapShape/IMapLine.h>
#include <Inner/ImplMapSceneNode.hpp>

class CMapLine:public ImplMapSceneNode<IMapLine>
{
public:
    CONSTRUCTOR(CMapLine,ImplMapSceneNode<IMapLine>)


    /**
     * @brief 增加点
     */
    virtual void AddPoint(int,const MapGeoPos&);

    /**
     * @brief 移除点
     */
    virtual bool RemovePoint(int);

    /**
     * @brief 更新指定位置
     */
    virtual bool UpdatePoint(int,const MapGeoPos&);

    /**
     * @brief 设置多个位置
     */
    void SetMultPos(const std::vector<MapGeoPos>&);
    std::vector<MapGeoPos> GetMulPos()const;
protected:
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

    /**
     * @brief 设置线宽
     */
    void LineWidthChanged();

    /**
     * @brief 设置线型
     */
    void LineTypeChanged();

    /**
     * @brief 设置线的颜色
     */
    void LineColorChanged();

private:
    osg::observer_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CSCENELINE_H
