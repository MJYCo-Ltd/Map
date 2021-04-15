#ifndef INTERFACE_BOX_SENSOR_HEARDER_H
#define INTERFACE_BOX_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 盒子传感器
 */
class IBoxSensor:public ISensor
{
public:
    CONSTRUCTOR(IBoxSensor,ISensor)

    /**
     * @brief 设置y/x轴的比率
     */
    void SetYXRadio(double dYXRatio)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dYXRatio,m_dYXRatio,ShapeChanged)
    double YXRadio() const{return(m_dYXRatio);}

    /**
     * @brief 设置z/x轴的比率
     */
    void SetZXRadio(double dZXRatio)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dZXRatio,m_dZXRatio,ShapeChanged)
    double ZXRadio() const{return(m_dZXRatio);}
protected:
    virtual ~IBoxSensor(){}

protected:
    double m_dYXRatio=1.;
    double m_dZXRatio=1.;
};

#endif
