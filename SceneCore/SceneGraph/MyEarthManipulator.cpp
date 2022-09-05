#include <Inner/OsgExtern/IOsgMapSceneNode.h>
#include <VersionMathCommon.h>
#include <Math/VecMat.h>
#include "MyEarthManipulator.h"

CMyEarthManipulator::CMyEarthManipulator(MapType type):
    osgEarth::Util::EarthManipulator()
{
    ChangeMap(type);
    _lastKnownVFOV=45;
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
        {
            getSettings()->setThrowingEnabled(true);
            getSettings()->setThrowDecayRate(0.009);
            getSettings()->setMinMaxDistance(100,DBL_MAX);
            getSettings()->setLockAzimuthWhilePanning(false);
            getSettings()->setTerrainAvoidanceEnabled(true);
            getSettings()->setTerrainAvoidanceMinimumDistance(1);
        }
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
    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::DOUBLECLICK:
        if(m_bAvoidDClick)
        {
            return true;
        }
        break;
    case osgGA::GUIEventAdapter::KEYDOWN:

        if(ea.getKey()==32 &&m_bLockView) //锁定视角排除空格事件
        {
            return true;
        }
        break;
    case osgGA::GUIEventAdapter::DRAG:
        if(m_bAvoidDrag && ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            return(true);
        }
        break;
    case osgGA::GUIEventAdapter::FRAME:
        if(m_bOpenEarthSelfRotate)
        {
            if(m_dPreTime<0)
            {
                m_dPreTime = ea.getTime();
            }
            else
            {
                double dOfftimeTime = ea.getTime() - m_dPreTime;
                static Math::CVector v3(3);
                v3.Set(_center.x(),_center.y(),_center.z());

                v3 = Math::CVecMat::R_z(dOfftimeTime* m_dScale * DS2R)*v3;
                _center.set(v3.GetX(),v3.GetY(),v3.GetZ());

                m_dPreTime = ea.getTime();
            }
        }
        break;
    }

    if(MAP_2D == m_emType && !m_bCalFactor)
    {
        m_dTanFvoy = tan(osg::DegreesToRadians(_lastKnownVFOV/2.));
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
    if(m_bLockView)
        return;
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
    if(m_bLockView)
        return;
    if(MAP_3D == m_emType)
    {
        osgEarth::Util::EarthManipulator::rotate(dx,dy);
    }
    else
    {
        return;
    }
}

#include <iostream>
void CMyEarthManipulator::zoom(double dx, double dy, osg::View *view)
{
    if(m_bLockView)
        return;
    osgEarth::Util::EarthManipulator::zoom(-dx,-dy,view);
    switch(m_emType)
    {
    case MAP_3D:
    {
        //        osgEarth::Viewpoint here = getViewpoint();
        //        if(dy>0)
        //        {
        //            here.range() = here.range().get() * 0.9;
        //        }
        //        else
        //        {
        //            here.range() = here.range().get() * 1.1;
        //        }
        //        setViewpoint( here, 0.5 );
    }
        break;
    case MAP_2D:
    {
        auto vp = getViewpoint();
        if(AdjustViewPoint(vp))
        {
            setViewpoint(vp);
        }
        break;
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
    vp.focalPoint()= osgEarth::GeoPoint(IOsgMapSceneNode::s_pWGS84,
                                        homePoint.stPos.dX,homePoint.stPos.dY,homePoint.stPos.dZ);
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
    vp.focalPoint()= osgEarth::GeoPoint(IOsgMapSceneNode::s_pWGS84,
                                        viewPoint.stPos.dX,viewPoint.stPos.dY,viewPoint.stPos.dZ);
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
