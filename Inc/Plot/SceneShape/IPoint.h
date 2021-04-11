#ifndef INTERFACE_POINT_HEARDER_H
#define INTERFACE_POINT_HEARDER_H
#include <string>
#include <Plot/SceneShape/IGeometry.h>

/**
 * @brief 场景点节点
 */
class IPoint:public IGeometry
{
public:
    CONSTRUCTOR(IPoint,IGeometry)

    /**
     * @brief 设置点的大小
     */
    void SetPointSize(float fPointSize)JUDGE_POSITIVE_FLOAT_CALL_FUNCTION(fPointSize,m_fPointSize,PointSizeChanged)
    float PointSize()const{return(m_fPointSize);}

    /**
     * @brief 设置点显示的图片
     */
    void SetImage(const std::string& sImagePath)JUDGE_EQUAL_CALL_FUNCTION(sImagePath,m_sImagePath,ImagePathChanged)
    const std::string& Image()const{return(m_sImagePath);}
protected:
    virtual ~IPoint(){}

    /// 状态更改
    virtual void PointSizeChanged()=0;
    virtual void ImagePathChanged()=0;

protected:
    float       m_fPointSize=1.f;
    std::string m_sImagePath;
};

#endif//INTERFACE_POINT_HEARDER_H
