#ifndef INTERFACE_CONE_SENSOR_HEARDER_H
#define INTERFACE_CONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 圆锥传感器
 */
class IConeSensor:public ISensor
{
public:
    CONSTRUCTOR(IConeSensor,ISensor)

    /**
     * @brief 设置圆锥半角
     */
    void SetAngle(double dAngle)JUDGE_DOUBLE_CALL_FUNCTION(dAngle,m_dAngle,ShapeChanged)
    double Angle() const{return(m_dAngle);}
protected:
    virtual ~IConeSensor(){}

protected:
    double m_dAngle=5.0;
};

#endif
