#ifndef INTERFACE_POINT_HEARDER_H
#define INTERFACE_POINT_HEARDER_H
#include <string>
#include <Plot/SceneShape/IShape.h>

/**
 * @brief 场景点节点
 */
class IPoint:public IShape
{
public:
    CONSTRUCTOR(IPoint,IShape)

    /**
     * @brief 设置点的位置
     */
    void SetPos(const ScenePos& rPos)JUDGE_EQUAL_CALL_FUNCTION(rPos,m_stPos,PosChanged)
    const ScenePos& Pos()const{return(m_stPos);}

    /**
     * @brief 设置点的大小
     */
    void SetPointSize(float fPointSize){if(fPointSize>0.f && fabs(fPointSize-m_fPointSize)>FLT_EPSILON){m_fPointSize=fPointSize;PointSizeChanged();}}
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
    virtual void PosChanged()=0;
    virtual void ImagePathChanged()=0;

protected:
    ScenePos m_stPos;
    float    m_fPointSize=1.f;
    std::string m_sImagePath;
};

#endif//INTERFACE_POINT_HEARDER_H
