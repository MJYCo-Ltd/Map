#ifndef INTERFACE_PULSE_SENSOR_HEARDER_H
#define INTERFACE_PULSE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief脉冲传感器
 */
class IPulseSensor:public ISensor
{
public:
    virtual ~IPulseSensor(){}

    /**
     * @brief 设置起始半径
     * @param dRadius
     */
    virtual void SetStartRadius(const double&)=0;
    virtual double GetStartRadius() const =0;

    /**
     * @brief 设置结束半径
     * @param dEndRadius
     */
    virtual void SetEndRadius(const double&)=0;
    virtual double GetEndRadius()const = 0;

    /**
     * @brief 设置整个过程中脉冲的数量
     * @param nCount
     */
    virtual void SetPulseCount(const int&)=0;

    /**
     * @brief 设置结束位置
     */
    virtual void SetEndPos(const ScenePos&)=0;

};

#endif
