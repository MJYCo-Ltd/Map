#ifndef INTERFACE_CONE_SENSOR_HEARDER_H
#define INTERFACE_CONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 场景节点类
 */
class IConeSensor:public ISensor
{
public:
    virtual ~IConeSensor(){}

    /**
     * @brief
     */
    virtual void SetAngle(double)=0;
    virtual double Angle() const =0;
};

#endif
