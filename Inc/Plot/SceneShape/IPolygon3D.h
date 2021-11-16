#ifndef INTERFACE_POLYGON_3D_HEARDER_H
#define INTERFACE_POLYGON_3D_HEARDER_H

#include <Plot/SceneShape/IGeometry.h>

/**
 * @brief 多面体
 */
class IPolygon3D:public IGeometry
{
public:
    CONSTRUCTOR(IPolygon3D,IGeometry)

    /**
     * @brief 设置多边形的高度
     */
    void SetHeight(double dHeight)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dHeight,m_dHeight,HeightChanged)
    double Height(){return(m_dHeight);}

    /**
     * @brief 设置是否显示底边
     */
    void ShowBottom(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowBottom,ShapeChanged);
    bool IsBottomShow(){return(m_bShowBottom);}
protected:
    virtual void HeightChanged()=0;
    virtual ~IPolygon3D(){}
protected:
    double m_dHeight{1.}; /// 高度
    bool   m_bShowBottom{true};
};

#endif//INTERFACE_POLYGON_3D_HEARDER_H
