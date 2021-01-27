#ifndef INTERFACE_SENSOR_HEARDER_H
#define INTERFACE_SENSOR_HEARDER_H
#include <Plot/SceneShape/IShape.h>

/**
 * @brief 传感器基类
 */
class ISensor:public IShape
{
public:
    CONSTRUCTOR(ISensor,IShape)

    /**
     * @brief 显示面
     */
    void ShowFace(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowFace,ShowTypeChanged)
    bool IsShowFace()const{return(m_bShowFace);}

    /**
     * @brief 显示线
     */
    void ShowLine(bool bShow)JUDGE_EQUAL_CALL_FUNCTION(bShow,m_bShowLine,ShowTypeChanged)
    bool IsShowLine()const{return(m_bShowLine);}

    /**
     * @brief 设置探测距离
     */
    void SetDistance(double dDistance)JUDGE_DOUBLE_CALL_FUNCTION(dDistance,m_dDistance,DistanceChanged)
    double Distance()const{return(m_dDistance);}

protected:
    virtual ~ISensor(){}

    virtual void DistanceChanged()=0;
    virtual void ShowTypeChanged()=0;
protected:
    bool       m_bShowLine=true;
    bool       m_bShowFace=true;
    double     m_dDistance=1000.;
};

#endif
