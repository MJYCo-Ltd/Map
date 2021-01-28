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
    void PosChanged(){m_bPosChanged=true;ShapeChanged();}

    /**
     * @brief 点大小更改
     */
    void PointSizeChanged(){m_bPointSizeChanged=true;ShapeChanged();}

    /**
     * @brief 图片路径更改
     */
    void ImagePathChanged(){m_bImageChanged=true;ShapeChanged();}

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
