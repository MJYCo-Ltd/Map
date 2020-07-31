#include "MyEarthManipulator.h"

CMyEarthManipulator::CMyEarthManipulator(MapType type):
    osgEarth::Util::EarthManipulator()
{
    getSettings()->setThrowDecayRate(0.1);
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
            getSettings()->setMinMaxDistance(1,DBL_MAX);
            getSettings()->setLockAzimuthWhilePanning(false);
            getSettings()->setTerrainAvoidanceEnabled(true);
            break;
        case MAP_2D:
            getSettings()->setMinMaxDistance(10,m_dMaxDistance);
            AdjustViewPoint();
            break;
        }
    }
}

/// 处理处理消息
bool CMyEarthManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if(MAP_2D == m_emType && !m_bCalFactor)
    {
        auto view = us.asView();
        double dFvoy,dAccept,dNear,dFar;
        view->getCamera()->getProjectionMatrixAsPerspective(dFvoy,dAccept,dNear,dFar);

        m_dTanFvoy = tan(osg::DegreesToRadians(dFvoy/2.0));

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

        /// 设置视点位置
        setViewpoint(vp);

        AdjustViewPoint();
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
    osgEarth::Util::EarthManipulator::zoom(dx,dy,view);
    if(MAP_2D == m_emType)
    {
        AdjustViewPoint();
    }
}

/// 调整视点
void CMyEarthManipulator::AdjustViewPoint()
{
    if(!m_bInit)
    {
        return;
    }
    osgEarth::Viewpoint vp = getViewpoint();

    const double YOffet = vp.focalPoint()->y();
    const double EyeHeight = vp.range()->getValue();

    const double DISTANCE = m_extent.yMax()-EyeHeight*m_dFactor;
    if(YOffet > 0 && (YOffet + EyeHeight*m_dFactor) > m_extent.yMax())
    {
        vp.focalPoint()->y() = DISTANCE;
        setViewpoint(vp);
    }
    else if(YOffet < 0 && (-YOffet + EyeHeight*m_dFactor) > m_extent.yMax())
    {
        vp.focalPoint()->y() = -DISTANCE;
        setViewpoint(vp);
    }

}

/// 初始化视点
void CMyEarthManipulator::InitViewPoint()
{
    osgEarth::Viewpoint vp;
    auto srs = osgEarth::SpatialReference::get("wgs84");


    vp.focalPoint()= osgEarth::GeoPoint(srs,118.8,32.1,0);
    vp.heading()->set(0,osgEarth::Units::DEGREES);
    vp.pitch()->set(-90,osgEarth::Units::DEGREES);
    vp.range()->set(12000,osgEarth::Units::KILOMETERS);
    setHomeViewpoint(vp);

    if(MAP_2D == m_emType)
    {
        AdjustViewPoint();
    }
}
