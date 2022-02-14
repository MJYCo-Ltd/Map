#ifndef CRADARSENSOR_H
#define CRADARSENSOR_H
#include <Plot/SceneShape/IRadarSensor.h>
#include <Inner/ImplSceneSensor.hpp>

struct FlankPoint
{
    double dX;    /// 与中心点所成的角度
    double dZ;    /// 到中心点的距离
};
typedef std::vector<FlankPoint> Flank;

class CRadarSensor:public ImplSceneSensor<IRadarSensor>
{
public:
    CONSTRUCTOR(CRadarSensor,ImplSceneSensor<IRadarSensor>)

    /**
     * @brief 设置雷达数据
     */
    void SetRadarData(const std::string& sFilePath,bool bIsRef);
protected:
    void CreateShape();
    void UpdateShape();

    void DealBuffer(const char *pBuffer);
    void CreateGeomery(double dRad, const Flank &vFlank);
protected:
    double m_dMaxDis{};
};

#endif // CRADARSENSOR_H
