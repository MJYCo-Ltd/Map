#include "MyEarthManipulator.h"

CMyEarthManipulator::CMyEarthManipulator(MapType type):
    osgEarth::Util::EarthManipulator()
{
    ChangeMap(type);
}

///
void CMyEarthManipulator::ChangeMap(MapType emType)
{
    if(emType == m_emType)
    {
        return;
    }
    else
    {
        m_emType = emType;

        switch (m_emType)
        {
        case MAP_3D:
            getSettings()->setThrowingEnabled(true);
            getSettings()->setThrowDecayRate(0.009);
            getSettings()->setMinMaxDistance(100,DBL_MAX);
            getSettings()->setLockAzimuthWhilePanning(false);
            getSettings()->setTerrainAvoidanceEnabled(true);
            getSettings()->setTerrainAvoidanceMinimumDistance(1);
            break;
        case MAP_2D:
            getSettings()->setMinMaxDistance(100,m_dMaxDistance);
            auto vp = getViewpoint();
            if(AdjustViewPoint(vp))
            {
                setViewpoint(vp);
            }
            break;
        }
    }
}

/// 处理处理消息
bool CMyEarthManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if(MAP_2D == m_emType && !m_bCalFactor)
    {
        m_dTanFvoy = tan(osg::DegreesToRadians(15.));
        m_bCalFactor = true;
    }

    if(!m_bInit && MAP_2D == m_emType && _mapNode.valid() && _mapNode->getMap())
    {
        m_extent = _mapNode->getMap()->getProfile()->getExtent();
        m_dHalfY = m_extent.yMax();
        m_dMaxDistance = m_dHalfY / m_dTanFvoy;
        getSettings()->setMinMaxDistance(10,m_dMaxDistance);
        m_dFactor = m_dTanFvoy;
        m_bInit = true;
    }

    return(osgEarth::Util::EarthManipulator::handle(ea,us));
}


void CMyEarthManipulator::pan(double dx, double dy)
{
    osgEarth::Util::EarthManipulator::pan(dx,dy);

    if(MAP_2D == m_emType)
    {
        /// 根据视点判断位置
        osgEarth::Viewpoint vp=getViewpoint();

        /// 如果视点位置超过X方向的范围
        if (vp.focalPoint()->x()<m_extent.xMin())
        {
            vp.focalPoint()->x() +=m_extent.width();
        }
        else if (vp.focalPoint()->x()>m_extent.xMax())
        {
            vp.focalPoint()->x() -=m_extent.width();
        }

        /// 如果视点位置超过Y方向范围
        if(vp.focalPoint()->y() > m_extent.yMax())
        {
            vp.focalPoint()->y() = m_extent.yMax();
        }
        else if(vp.focalPoint()->y() < m_extent.yMin())
        {
            vp.focalPoint()->y() = m_extent.yMin();
        }

        AdjustViewPoint(vp);
        /// 设置视点位置
        setViewpoint(vp);
    }
}

void CMyEarthManipulator::rotate(double dx, double dy)
{
    if(MAP_3D == m_emType)
    {
        osgEarth::Util::EarthManipulator::rotate(dx,dy);
    }
    else
    {
        return;
    }
}

void CMyEarthManipulator::zoom(double dx, double dy, osg::View *view)
{
    osgEarth::Util::EarthManipulator::zoom(-dx,-dy,view);
    if(MAP_2D == m_emType)
    {
        auto vp = getViewpoint();
        if(AdjustViewPoint(vp))
        {
            setViewpoint(vp);
        }
    }
}

/// 调整视点
bool CMyEarthManipulator::AdjustViewPoint(osgEarth::Viewpoint& vp)
{
    if(!m_bInit)
    {
        return(false);
    }

    const double YOffet = vp.focalPoint()->y();
    const double EyeHeight = vp.range()->getValue();

    const double DISTANCE = m_extent.yMax()-EyeHeight*m_dFactor;
    if(YOffet > 0 && (YOffet + EyeHeight*m_dFactor) > m_extent.yMax())
    {
        vp.focalPoint()->y() = DISTANCE;
        return(true);
    }
    else if(YOffet < 0 && (-YOffet + EyeHeight*m_dFactor) > m_extent.yMax())
    {
        vp.focalPoint()->y() = -DISTANCE;
        return(true);
    }

    return(false);
}

/// 初始化视点
void CMyEarthManipulator::InitHomePoint(const SceneViewPoint &homePoint)
{
    osgEarth::Viewpoint vp;
    vp.focalPoint()= osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),
                                        homePoint.stPos.fX,homePoint.stPos.fY,homePoint.stPos.fZ);
    vp.heading()->set(homePoint.fAzimuth,osgEarth::Units::DEGREES);
    vp.range()->set(homePoint.fDistance,osgEarth::Units::METERS);

    if(MAP_2D == m_emType)
    {
        AdjustViewPoint(vp);
    }
    else
    {
        vp.pitch()->set(-homePoint.fElev,osgEarth::Units::DEGREES);
    }

    setHomeViewpoint(vp);
}

/// 设置视点
void CMyEarthManipulator::SetViewPoint(const SceneViewPoint &viewPoint,double dTimes)
{
    osgEarth::Viewpoint vp;
    vp.focalPoint()= osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),
                                        viewPoint.stPos.fX,viewPoint.stPos.fY,viewPoint.stPos.fZ);
    vp.heading()->set(viewPoint.fAzimuth,osgEarth::Units::DEGREES);
    vp.range()->set(viewPoint.fDistance,osgEarth::Units::METERS);

    if(MAP_2D == m_emType)
    {
        AdjustViewPoint(vp);
    }
    else
    {
        vp.pitch()->set(-viewPoint.fElev,osgEarth::Units::DEGREES);
    }
    setViewpoint(vp,dTimes);
}
