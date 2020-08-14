#ifndef H_HEADER_CPOINT_H
#define H_HEADER_CPOINT_H

#include <osgEarth/FeatureNode>
#include <Map/Plot/IPoint.h>
#include <Inner/QtDrawShape.h>
class PointCallBack;

class CScenePoint:public QtDrawShape<IPoint>
{
    friend class PointCallBack;
public:
    CScenePoint(ISceneGraph*pSceneGraph);

    /**
     * @brief 更新地图节点
     * @param pMapNode
     */
    void UpdateMapNode(osgEarth::MapNode* pMapNode);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 位置更新消息
     */
    void PosChanged();

    /**
     * @brief 初始化样式
     */
    void InitStyle();

    /**
     * @brief 设置点的大小
     */
    void SetPointSize(float);
    const float& GetPointSize(){return(m_fPointSize);}

    /**
     * @brief 设置颜色
     */
    void SetColor(const SceneColor&);
    const SceneColor& GetColor(){return (m_stColor);}
protected:
    float  m_fPointSize=10.0f;
    bool   m_bUpdate=false;
    SceneColor m_stColor;
    osg::ref_ptr<PointCallBack> m_pPointCallBack;
    osg::ref_ptr<osgEarth::FeatureNode> m_pFeatureNode;
};

#endif // CPOINT_H
