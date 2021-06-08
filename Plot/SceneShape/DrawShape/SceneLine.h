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
     * @brief 线型更改
     */
    void LineTypeChanged()SET_TRUE_NODE_UPDATE(m_bLineTypeChanged)

    /**
     * @brief 荧光状态更改
     */
    void GlowChanged()SET_TRUE_NODE_UPDATE(m_bGlowChanged)

    /**
     * @brief 构造模型
     */
    void CreateShape();

    /**
     * @brief 更新形状
     */
    void UpdateShape();
protected:
    osg::ref_ptr<osg::Uniform> m_uLineWidth;
    osg::ref_ptr<osg::Uniform> m_uCameraSize;
    osg::ref_ptr<osg::Vec3Array> m_pPreviousPoints;
    osg::ref_ptr<osg::Vec3Array> m_pNextPoints;
    bool                       m_bWidthChanged{false};
    bool                       m_bLineTypeChanged{false};
    bool                       m_bGlowChanged{false};
};

#endif // CSCENELINE_H
