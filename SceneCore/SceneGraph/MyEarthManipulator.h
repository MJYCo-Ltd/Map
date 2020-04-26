#ifndef CFLATEARTHMANIPULATOR_H
#define CFLATEARTHMANIPULATOR_H

#include <osgEarth/Version>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/EarthManipulator>
#else
#include <osgEarthUtil/EarthManipulator>
#endif

#include "Map/IMap.h"

class CMyEarthManipulator : public osgEarth::Util::EarthManipulator
{
public:
    CMyEarthManipulator(MapType type);

    /**
     * @brief 切换地图类型
     * @param emType
     */
    void ChangeMap(MapType emType);

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
    void pan(double dx, double dy);
    void rotate(double dx, double dy);
    void zoom( double dx, double dy );

    /**
     * @brief 初始化视点
     */
    void InitViewPoint();

protected:

    /**
     * @brief 调整视点
     */
    void AdjustViewPoint();

private:
    MapType m_emType;
    osgEarth::GeoExtent m_extent;
    double m_dHalfY;
    double m_dFactor;
    double m_dTanFvoy=DBL_MIN;
    double m_dMaxDistance = 360000000;
    bool   m_bCalFactor=false;
    bool   m_bInit=false;
};

#endif // CFLATEARTHMANIPULATOR_H
