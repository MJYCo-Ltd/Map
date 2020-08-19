#ifndef INTERFACE_SCONE_SENSOR_HEARDER_H
#define INTERFACE_SCONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 场景节点类
 */
class ISConeSensor:public ISensor
{
public:
    virtual ~ISConeSensor(){}

    /**
     * @brief
     */
    virtual void SetHAngle(double)=0;
    virtual double HAngle() const =0;

    virtual void SetVAngle(double)=0;
    virtual double VAngle()const=0;
};

#endif
