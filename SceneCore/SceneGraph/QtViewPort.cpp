#include <osgEarth/GeoData>
#include <osgViewer/ViewerEventHandlers>

#include <Inner/IRender.h>
#include <Inner/IOsgSceneNode.h>

#include "MyEarthManipulator.h"
#include "QtViewPort.h"
#include "QtViewHud.h"
#include "DealViewPortChange.h"
/// 视点
QtViewPort::QtViewPort(IRender *pRender,ISceneGraph *pSceneGraph, ProjectType emProject):
    m_pSceneGraph(pSceneGraph),
    m_pRender(pRender),
    m_emProjectType(emProject)
{
    m_pView = new osgViewer::View;

    m_pView->getCamera()->setViewport(0,0,C_WINDOW_WIDTH,C_WINDOW_HEIGHT);
    m_pView->getCamera()->setClearColor(osg::Vec4(0,0,0,1));
    switch(m_emProjectType)
    {
    case Perspective:
        m_pView->getCamera()->setProjectionMatrixAsPerspective(45,1.0f,0.01,100.);
        break;
    case Ortho:
        m_pView->getCamera()->setProjectionMatrixAsOrtho2D(0,C_WINDOW_WIDTH,0,C_WINDOW_HEIGHT);
        break;
    }
    m_pCameraUpdate = new QtViewPointUpdateCallback(this);

    m_pView->getCamera()->addUpdateCallback(m_pCameraUpdate);
    m_pView->addEventHandler(new osgViewer::StatsHandler);

    m_pSelfManipulator = new osgGA::TrackballManipulator;
    m_pView->setCameraManipulator(m_pSelfManipulator);
}

/// 析构函数
QtViewPort::~QtViewPort()
{
    delete m_pHud;
    m_pView->getCamera()->removeUpdateCallback(m_pCameraUpdate);
}

/// 地图类型切换
void QtViewPort::MapTypeChanged(MapType emType)
{
    switch(emType)
    {
    case MAP_2D:
    {
        if(!m_p2DEarthManipulator.valid())
        {
            m_p2DEarthManipulator = new CMyEarthManipulator(emType);
            m_p2DEarthManipulator->InitViewPoint();
        }
        m_emType = View_2D;
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
    }
        break;
    case MAP_3D:
        if(!m_p3DEarthManipulator.valid())
        {
            m_p3DEarthManipulator = new CMyEarthManipulator(emType);
            m_p3DEarthManipulator->InitViewPoint();
        }
        m_emType = View_3D;
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
        break;
    }
}

/// 设置跟踪视点
bool QtViewPort::SetTrackNode(ISceneNode *pTrackNode)
{
    if(m_pSceneGraph != pTrackNode->GetBoundSceneGraph())
    {
        return(false);
    }

    IOsgSceneNode* pOsgNode = pTrackNode->AsOsgSceneNode();

    if(pOsgNode)
    {
//        osgEarth::Viewpoint viewPoint = m_p3DEarthManipulator->getViewpoint();
//        viewPoint.setNode(pOsgNode->GetOsgNode());
//        viewPoint.setRange(osgEarth::Distance(1000,osgEarth::Units::METERS));
//        m_p3DEarthManipulator->setViewpoint(viewPoint,3);
        if(!m_pTrackManipulator.valid())
        {
            m_pTrackManipulator = new osgGA::NodeTrackerManipulator;
            m_pTrackManipulator->setDistance(m_rViewPoint.fDistance);

            m_emPreType = m_emType;
            m_emType = View_Node;

            m_pTrackNode = pOsgNode;
            m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pTrackManipulator));
        }
        else
        {
            m_pTrackManipulator->setRotationMode(osgGA::NodeTrackerManipulator::ELEVATION_AZIM);
            if(pOsgNode != m_pTrackNode)
            {
                m_emPreType = m_emType;
                m_emType = View_Node;
                m_pTrackNode = pOsgNode;
                m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
            }
        }
    }
    else
    {
        switch (m_emPreType)
        {
        case View_3D:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
            break;
        case View_2D:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
            break;
        default:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
            break;
        }
    }

    return(true);
}

/// 获取场景根节点
ISceneNode *QtViewPort::GetTrackNode()
{
    return(dynamic_cast<ISceneNode*>(m_pTrackNode));
}

IViewHud *QtViewPort::GetHud()
{
    if(nullptr == m_pHud)
    {
        m_pHud = new QtViewHud(m_pView,m_pSceneGraph);
        m_pHud->Init();
        m_pSceneGraph->GetRoot()->AddSceneNode(m_pHud);
    }
    return(m_pHud);
}

/// 设置视点
void QtViewPort::SetViewPoint(const SceneViewPoint & rViewPoint)
{
    if(rViewPoint != m_rViewPoint)
    {
        m_rViewPoint = rViewPoint;

        switch(m_emType)
        {
        case View_3D:
        {
            auto viewPoint = m_p3DEarthManipulator->getViewpoint();
            viewPoint.setFocalPoint(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),m_rViewPoint.stPos.fX,
                                                       m_rViewPoint.stPos.fY,m_rViewPoint.stPos.fZ));
            viewPoint.setRange(osgEarth::Distance(m_rViewPoint.fDistance,osgEarth::Units::METERS));
            viewPoint.setHeading(osgEarth::Angle(m_rViewPoint.fAzimuth,osgEarth::Units::DEGREES));
            viewPoint.setPitch(osgEarth::Angle(0-m_rViewPoint.fElev,osgEarth::Units::DEGREES));

            m_p3DEarthManipulator->setViewpoint(viewPoint,3);
        }
            break;
        case View_2D:
        {
            auto viewPoint = m_p2DEarthManipulator->getViewpoint();
            viewPoint.setFocalPoint(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),m_rViewPoint.stPos.fX,
                                                       m_rViewPoint.stPos.fY,m_rViewPoint.stPos.fZ));
            viewPoint.setRange(osgEarth::Distance(m_rViewPoint.fDistance,osgEarth::Units::METERS));

            m_p2DEarthManipulator->setViewpoint(viewPoint,3);
        }
            break;
        case View_Node:
        {
            m_pTrackManipulator->setDistance(m_rViewPoint.fDistance);
            m_pTrackManipulator->setHeading(osg::DegreesToRadians(m_rViewPoint.fAzimuth));
            m_pTrackManipulator->setElevation(osg::DegreesToRadians(m_rViewPoint.fElev));
        }
            break;
        case View_User:
            break;
        default:
            break;
        }
    }
}

/// 获取视点位置
const SceneViewPoint &QtViewPort::GetViewPoint()
{
    return(m_rViewPoint);
}

/// 设置视口
void QtViewPort::SetViewPort(const CameraViewPort &rViewPort)
{
    if(m_rViewPort != rViewPort)
    {
        m_rViewPort = rViewPort;

        m_pCameraUpdate->UpdateViewPort();
    }
}

/// 获取窗口大小
const CameraViewPort &QtViewPort::GetViewPort()
{
    return(m_rViewPort);
}

/// 设置投影类型
void QtViewPort::SetProjectType(ProjectType emProject)
{
    if(emProject != m_emProjectType)
    {
        m_emProjectType = emProject;
        m_pCameraUpdate->UpdateProject();
    }
}

/// 返回投影类型
ProjectType QtViewPort::GetProjectType()
{
    return(m_emProjectType);
}

/// 获取视图类
osgViewer::View *QtViewPort::GetOsgView()
{
    return(m_pView.get());
}
