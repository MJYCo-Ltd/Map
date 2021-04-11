#ifndef CSCENEPOINT_H
#define CSCENEPOINT_H
#include <Inner/ImplSceneGeometry.hpp>
#include <Plot/SceneShape/IPoint.h>
class CScenePoint:public ImplSceneGeometry<IPoint>
{
public:
    CONSTRUCTOR(CScenePoint,ImplSceneGeometry<IPoint>)

protected:
    /**
     * @brief 点大小更改
     */
    void PointSizeChanged() override SET_TRUE_SHAPE_UPDATE(m_bPointSizeChanged)

    /**
     * @brief 图片路径更改
     */
    void ImagePathChanged() override SET_TRUE_SHAPE_UPDATE(m_bImageChanged)

    /**
     * @brief 更新形状
     */
    void UpdateShape() override;

    /**
     * @brief 构造模型
     */
    void CreateShape() override;
protected:
    osg::ref_ptr<osg::Uniform> m_ufPointSize;
    bool     m_bPointSizeChanged=false;
    bool     m_bImageChanged=false;
};

#endif // CSCENEPOINT_H
