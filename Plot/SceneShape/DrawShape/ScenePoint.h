#ifndef CSCENEPOINT_H
#define CSCENEPOINT_H
#include <Inner/ImplSceneShape.hpp>
#include <Plot/SceneShape/IPoint.h>
class CScenePoint:public ImplSceneShape<IPoint>
{
public:
    CONSTRUCTOR(CScenePoint,ImplSceneShape<IPoint>)

protected:

    /**
     * @brief 位置更改
     */
    void PosChanged()SET_TRUE_SHAPE_UPDATE(m_bPosChanged)

    /**
     * @brief 点大小更改
     */
    void PointSizeChanged()SET_TRUE_SHAPE_UPDATE(m_bPointSizeChanged)

    /**
     * @brief 图片路径更改
     */
    void ImagePathChanged()SET_TRUE_SHAPE_UPDATE(m_bImageChanged)

    /**
     * @brief 更新形状
     */
    void UpdateShape();

    /**
     * @brief 构造模型
     */
    void CreateShape();
protected:
    osg::ref_ptr<osg::Uniform> m_ufPointSize;
    bool     m_bPointSizeChanged=false;
    bool     m_bPosChanged=false;
    bool     m_bImageChanged=false;
};

#endif // CSCENEPOINT_H
