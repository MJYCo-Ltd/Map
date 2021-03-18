#include <osg/Stencil>
#include <osg/PolygonStipple>
#include <osgEarth/GeoData>
#include <osgViewer/ViewerEventHandlers>

#include <Inner/IRender.h>
#include <Inner/IOsgSceneNode.h>

#include "MyEarthManipulator.h"
#include "QtViewPort.h"
#include "QtViewHud.h"
#include "DealViewPortChange.h"
class ViewPortEventCallback:public osgGA::GUIEventHandler
{
public:
    ViewPortEventCallback(QtViewPort* pViewPort):m_pViewPort(pViewPort){}
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&,
                        osg::Object*, osg::NodeVisitor*)
    {
        if(ea.FRAME == ea.getEventType())
        {
            m_pViewPort->FrameEvent();
        }
        return(false);
    }

protected:
    QtViewPort* m_pViewPort;
};

/// 视点
QtViewPort::QtViewPort(IRender *pRender,ISceneGraph *pSceneGraph, ProjectType emProject):
    m_pSceneGraph(pSceneGraph),
    m_pRender(pRender),
    m_emProjectType(emProject)
{
    m_pView = new osgViewer::View;

    m_pView->getCamera()->setViewport(0,0,C_WINDOW_WIDTH,C_WINDOW_HEIGHT);
    m_pView->getCamera()->setClearColor(osg::Vec4(0,0,0,1));

    m_pView->getCamera()->setDrawBuffer(GL_BACK);
    m_pView->getCamera()->setReadBuffer(GL_BACK);
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
    m_pView->addEventHandler(new ViewPortEventCallback(this));

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
ISceneNode *QtViewPort::GetTrackNode()const
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
void QtViewPort::SetViewPoint(const SceneViewPoint & rViewPoint, unsigned int unTimes)
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

        m_p3DEarthManipulator->setViewpoint(viewPoint,unTimes);
    }
        break;
    case View_2D:
    {
        auto viewPoint = m_p2DEarthManipulator->getViewpoint();
        viewPoint.setFocalPoint(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),m_rViewPoint.stPos.fX,
                                                   m_rViewPoint.stPos.fY,m_rViewPoint.stPos.fZ));
        viewPoint.setRange(osgEarth::Distance(m_rViewPoint.fDistance,osgEarth::Units::METERS));

        m_p2DEarthManipulator->setViewpoint(viewPoint,unTimes);
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

/// 获取视点位置
const SceneViewPoint &QtViewPort::GetViewPoint() const
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
const CameraViewPort &QtViewPort::GetViewPort()const
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
ProjectType QtViewPort::GetProjectType()const
{
    return(m_emProjectType);
}

/// 获取视图类
osgViewer::View *QtViewPort::GetOsgView()
{
    return(m_pView.get());
}

void QtViewPort::FrameEvent()
{
    if(m_bStereoChanged)
    {
        RemoveSlave();

        osg::DisplaySettings* ds= m_pView->getDisplaySettings();
        if(nullptr == ds)
        {
            ds =new osg::DisplaySettings(*osg::DisplaySettings::instance());
            m_pView->setDisplaySettings(ds);
//            ds->setSplitStereoAutoAdjustAspectRatio(true);
        }

        ds->setStereo(false);
        if(m_bOpenStereo)
        {
            ds->setStereo(true);
            ds->setStereoMode(osg::DisplaySettings::StereoMode(m_emStereo));

            int nWidth(C_WINDOW_WIDTH),nHeight(C_WINDOW_HEIGHT);
            auto pViewport = m_pView->getCamera()->getViewport();
            if(pViewport)
            {
                nWidth=pViewport->width();
                nHeight=pViewport->height();
            }

            auto gc = m_pView->getCamera()->getGraphicsContext();
            switch(m_emStereo)
            {
            case QUAD_BUFFER:
            {
                osg::ref_ptr<osg::Camera> left_camera = m_pView->assignStereoCamera(ds, gc,0, 0, nWidth, nHeight, GL_BACK_LEFT, -1.0);
                left_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                left_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 0);

                m_listStereoCamera.push_back(left_camera);
                osg::ref_ptr<osg::Camera> right_camera = m_pView->assignStereoCamera(ds, gc,0, 0, nWidth, nHeight,GL_BACK_RIGHT, 1.0);
                right_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                right_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 1);
                m_listStereoCamera.push_back(right_camera);
                break;
            }
            case ANAGLYPHIC:
            {
                osg::ref_ptr<osg::Camera> left_camera = m_pView->assignStereoCamera(ds, gc,0, 0,nWidth, nHeight, GL_BACK, -1.0);
                left_camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                left_camera->getOrCreateStateSet()->setAttribute(new osg::ColorMask(true, false, false, true));
                left_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 0);

                m_listStereoCamera.push_back(left_camera);

                // right Camera right buffer
                osg::ref_ptr<osg::Camera> right_camera = m_pView->assignStereoCamera(ds, gc, 0, 0,nWidth, nHeight, GL_BACK, 1.0);
                right_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
                right_camera->getOrCreateStateSet()->setAttribute(new osg::ColorMask(false, true, true, true));
                right_camera->setRenderOrder(osg::Camera::NESTED_RENDER, 1);
                m_listStereoCamera.push_back(right_camera);
                break;
            }
            case HORIZONTAL_SPLIT:
            {
                bool left_eye_left_viewport = ds->getSplitStereoHorizontalEyeMapping()==osg::DisplaySettings::LEFT_EYE_LEFT_VIEWPORT;
                int left_start = left_eye_left_viewport ? 0 : nWidth/2;
                int right_start = left_eye_left_viewport ? nWidth/2 : 0;

                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,left_start,0,nWidth/2,nHeight,GL_BACK,-1.0));
                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,right_start,0,nWidth/2,nHeight,GL_BACK,1.0));
                break;
            }
            case VERTICAL_SPLIT:
            {
                bool left_eye_bottom_viewport = ds->getSplitStereoVerticalEyeMapping()==osg::DisplaySettings::LEFT_EYE_BOTTOM_VIEWPORT;
                int left_start = left_eye_bottom_viewport ? 0 : nHeight/2;
                int right_start = left_eye_bottom_viewport ? nHeight/2 : 0;

                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,0,left_start,nWidth,nHeight/2,GL_BACK,-1.0));
                m_listStereoCamera.push_back(m_pView->assignStereoCamera(ds,gc,0,right_start,nWidth,nHeight/2,GL_BACK,1.0));
                break;
            }
            }
            m_pView->getCamera()->setGraphicsContext(0);
        }
        m_bStereoChanged=false;
    }
}

void QtViewPort::RemoveSlave()
{
    if(m_listStereoCamera.size() > 0)
    {
        std::cout<<m_pView->getNumSlaves()<<std::endl;
        m_pView->getCamera()->setGraphicsContext(m_listStereoCamera.front()->getGraphicsContext());
        for(auto one : m_listStereoCamera)
        {
            m_pView->removeSlave(m_pView->findSlaveIndexForCamera(one));
        }

        std::cout<<"Clear"<<m_pView->getNumSlaves()<<std::endl;
        m_listStereoCamera.clear();
    }
}
