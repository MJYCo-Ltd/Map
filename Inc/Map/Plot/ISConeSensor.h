#ifndef INTERFACE_SCONE_SENSOR_HEARDER_H
#define INTERFACE_SCONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 方锥传感器
 */
class ISConeSensor:public ISensor
{
public:
    virtual ~ISConeSensor(){}

    /**
     * @brief 设置运动垂向半角
     */
    virtual void SetHAngle(double)=0;
    virtual double HAngle() const =0;

    /**
     * @brief 设置运动方向上半角
     */
    virtual void SetVAngle(double)=0;
    virtual double VAngle()const=0;
};

#endif
