#ifndef INTERFACE_CONE_SENSOR_HEARDER_H
#define INTERFACE_CONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 圆锥传感器
 */
class IConeSensor:public ISensor
{
public:
    virtual ~IConeSensor(){}

    /**
     * @brief 设置圆锥半角
     */
    virtual void SetAngle(double)=0;
    virtual double Angle() const =0;
};

#endif
