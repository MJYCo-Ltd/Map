#ifndef INTERFACE_RADAR_SENSOR_HEARDER_H
#define INTERFACE_RADAR_SENSOR_HEARDER_H
#include <Plot/SceneShape/ISensor.h>

/**
 * @brief 雷达传感器
 */
class IRadarSensor:public ISensor
{
public:
    CONSTRUCTOR(IRadarSensor,ISensor)
    /**
     * @brief 设置雷达数据
     */
    virtual void SetRadarData(const std::string& sFilePath,bool bIsRef=true)=0;
protected:
    virtual ~IRadarSensor(){}
protected:
};

#endif
