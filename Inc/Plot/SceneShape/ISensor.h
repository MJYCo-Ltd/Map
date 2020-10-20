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
     * @brief 设置探测距离
     */
    virtual void SetDistance(double dDistance)JUDGE_DOUBLE_CALL_FUNCTION(dDistance,m_dDistance,DistanceChanged)
    virtual double Distance()const{return(m_dDistance);}
protected:
    virtual ~ISensor(){}

    virtual void DistanceChanged()=0;
protected:
    double     m_dDistance=1000.;
};

#endif
