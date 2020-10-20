#ifndef INTERFACE_SCONE_SENSOR_HEARDER_H
#define INTERFACE_SCONE_SENSOR_HEARDER_H
#include "ISensor.h"

/**
 * @brief 方锥传感器
 */
class ISConeSensor:public ISensor
{
public:
    ISConeSensor(ISceneGraph* pSceneGraph):ISensor(pSceneGraph){}
    /**
     * @brief 设置运动垂向半角
     */
    virtual void SetHAngle(double dHAngle)JUDGE_DOUBLE_CALL_FUNCTION(dHAngle,m_dHAngle,ShapeChanged)
    virtual double HAngle() const {return(m_dHAngle);}

    /**
     * @brief 设置运动方向上半角
     */
    virtual void SetVAngle(double dVAngle)JUDGE_DOUBLE_CALL_FUNCTION(dVAngle,m_dVAngle,ShapeChanged)
    virtual double VAngle()const{return(m_dVAngle);}
protected:
    virtual ~ISConeSensor(){}
protected:
    double  m_dHAngle=5.;
    double  m_dVAngle=5.;
};

#endif
