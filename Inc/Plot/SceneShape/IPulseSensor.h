#ifndef INTERFACE_PULSE_SENSOR_HEARDER_H
#define INTERFACE_PULSE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief脉冲传感器
 */
class IPulseSensor:public ISensor
{
public:

    CONSTRUCTOR(IPulseSensor,ISensor)

    /**
     * @brief 设置起始半径
     * @param dRadius
     */
    virtual void SetStartRadius(const double& dStartRadius)JUDGE_DOUBLE_CALL_FUNCTION(dStartRadius,m_dStartRadius,ShapeChanged)
    virtual double GetStartRadius() const{return(m_dStartRadius);}

    /**
     * @brief 设置结束半径
     * @param dEndRadius
     */
    virtual void SetEndRadius(double dEndRadius)JUDGE_DOUBLE_CALL_FUNCTION(dEndRadius,m_dEndRadius,ShapeChanged)
    virtual double GetEndRadius()const{return(m_dEndRadius);}

    /**
     * @brief 设置整个过程中脉冲的数量
     * @param nCount
     */
    virtual void SetPulseCount(int nCount)JUDGE_DOUBLE_CALL_FUNCTION(nCount,m_nCount,ShapeChanged)
    virtual int PulseCount(){return(m_nCount);}

protected:
    virtual ~IPulseSensor(){}

protected:
    double m_dStartRadius=100.;
    double m_dEndRadius=10000.;
    int    m_nCount=5;
};

#endif
