#ifndef INTERFACE_RADAR_SENSOR_HEARDER_H
#define INTERFACE_RADAR_SENSOR_HEARDER_H
#include <Plot/SceneShape/ISensor.h>

/**
 * @brief 传感器基类
 */
class IRadarSensor:public ISensor
{
public:
    CONSTRUCTOR(IRadarSensor,ISensor)

    /**
     * @brief 设置水平探测范围
     */
    void SetAzimuth(double dMin,double dMax)
    {
        if((dMax-dMin)>0 && (dMax-dMin) <= 360 && (fabs(dMin-m_dMinAzim)>DBL_EPSILON || fabs(dMax-m_dMaxAzim)>DBL_EPSILON))
        {
            m_dMinAzim=dMin;
            m_dMaxAzim=dMax;
            ShapeChanged();
        }
    }
    double MinAzimuth()const{return(m_dMinAzim);}
    double MaxAzimuth()const{return(m_dMaxAzim);}

    /**
     * @brief 设置垂直探测范围
     */
    void SetElevation(double dMin,double dMax)
    {
        if((dMax-dMin)>0 && (dMax-dMin) <= 180 && (fabs(dMin-m_dMinElev)>DBL_EPSILON || fabs(dMax-m_dMaxElev)>DBL_EPSILON))
        {
            m_dMinElev=dMin;
            m_dMaxElev=dMax;
            ShapeChanged();
        }
    }
    double MinElevation(){return(m_dMinAzim);}
    double MaxElevation(){return(m_dMaxAzim);}

protected:
    virtual ~IRadarSensor(){}
protected:
    double     m_dMinAzim=0.0;
    double     m_dMaxAzim=90.;
    double     m_dMinElev=0.0;
    double     m_dMaxElev=45.0;
};

#endif
