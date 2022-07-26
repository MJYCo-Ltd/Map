#ifndef CFLATEARTHMANIPULATOR_H
#define CFLATEARTHMANIPULATOR_H

#include <osgEarth/Version>

#if OSGEARTH_VERSION_GREATER_OR_EQUAL(3,0,0)
#include <osgEarth/EarthManipulator>
#else
#include <osgEarthUtil/EarthManipulator>
#endif

#include <Plot/Map/IMap.h>
#include <SceneGraph/ViewType.h>

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
    void zoom(double dx, double dy, osg::View *view);

    /**
     * @brief 忽略拖拽
     * @param bAvoid
     */
    void AvoidDrag(bool bAvoid);

    /**
     * @brief 初始化视点
     */
    void InitHomePoint(const SceneViewPoint& homePoint);

    /**
     * @brief 设置视点
     * @param viewPoint
     */
    void SetViewPoint(const SceneViewPoint& viewPoint, double dTimes);

    /**
     * @锁定视角
     */
    void SetLockView(bool);
protected:

    /**
     * @brief 调整视点
     */
    bool AdjustViewPoint(osgEarth::Viewpoint &vp);

private:
    MapType m_emType= MAP_USER;
    osgEarth::GeoExtent m_extent;
    double m_dHalfY{};
    double m_dFactor{};
    double m_dTanFvoy{DBL_MIN};
    double m_dMaxDistance{360000000};
    bool   m_bCalFactor{false};
    bool   m_bInit{false};
    bool   m_bLockView{false};
    bool   m_bAvoid{false};
};

#endif // CFLATEARTHMANIPULATOR_H
