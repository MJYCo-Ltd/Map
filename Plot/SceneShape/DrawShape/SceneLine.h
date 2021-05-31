#ifndef CSCENELINE_H
#define CSCENELINE_H
#include <list>
#include <osgEarth/LineDrawable>
#include <Plot/SceneShape/ILine.h>
#include <Inner/ImplSceneGeometry.hpp>
class CSceneLine:public ImplSceneGeometry<ILine>
{
public:
    CONSTRUCTOR(CSceneLine,ImplSceneGeometry<ILine>)

protected:

    /**
     * @brief 线宽更改
     */
    void LineWidthChanged()SET_TRUE_NODE_UPDATE(m_bWidthChanged)

    /**
     * @brief 形状更改
     */
    void ShapeChanged()SET_TRUE_NODE_UPDATE(m_bShapeChanged)

    /**
     * @brief 颜色修改
     */
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)

    /**
     * @brief 线型更改
     */
    void LineTypeChanged()SET_TRUE_NODE_UPDATE(m_bLineTypeChanged)

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 节点更新
     */
    void UpdateNode();

    void CreateShape(){}
    void UpdateShape(){}

    void NeedUpdate() SET_TRUE_NODE_UPDATE(m_bShapeChanged)
protected:
    osg::ref_ptr<osgEarth::LineDrawable> m_pLine;
    bool                          m_bColorChanged{false};
    bool                          m_bShapeChanged{false};
    bool                          m_bWidthChanged{false};
    bool                          m_bLineTypeChanged{false};
};

#endif // CSCENELINE_H
