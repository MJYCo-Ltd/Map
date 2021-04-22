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
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa,
                        osg::Object*, osg::NodeVisitor*)
    {
        if(ea.FRAME == ea.getEventType())
        {
            m_pViewPort->FrameEvent();
            auto view = aa.asView();
            if(nullptr != view)
            {

                view->getCamera()->getViewMatrixAsLookAt(m_vEye,m_vCenter,m_vUp);

                QMetaObject::invokeMethod(m_pViewPort,"EyePos",Q_ARG(double,m_vEye.x()),
                                          Q_ARG(double,m_vEye.y()),Q_ARG(double,m_vEye.z()));
                QMetaObject::invokeMethod(m_pViewPort,"LookDir",Q_ARG(double,m_vCenter.x()),
                                          Q_ARG(double,m_vCenter.y()),Q_ARG(double,m_vCenter.z()));
            }
        }

        return(osgGA::GUIEventHandler::handle(ea,aa));
    }

protected:
    QtViewPort* m_pViewPort;
    osg::Vec3d  m_vEye;
    osg::Vec3d  m_vCenter;
    osg::Vec3d  m_vUp;
};

/// 视点
QtViewPort::QtViewPort(IRender *pRender,ISceneGraph *pSceneGraph, ProjectType emProject):
    m_pSceneGraph(pSceneGraph),
    m_pRender(pRender)
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

    /// 默认的
    m_stHomePoint.fAzimuth = 0.f;
    m_stHomePoint.fElev = 0.f;
    m_stHomePoint.fDistance = 12000e3;
    m_stHomePoint.stPos.fX = 118.8f;
    m_stHomePoint.stPos.fY = 32.1f;
}

/// 析构函数
QtViewPort::~QtViewPort()
{
    delete m_pHud;
    m_pView->getCamera()->removeUpdateCallback(m_pCameraUpdate);
}

/// 地图类型切换
void QtViewPort::ViewPointTypeChanged(ViewPointType emType)
{
    if(emType == m_emType)
    {
        return;
    }

    switch(emType)
    {
    case View_2DMap:
        if(!m_p2DEarthManipulator.valid())
        {
            m_p2DEarthManipulator = new CMyEarthManipulator(MAP_2D);
            m_p2DEarthManipulator->InitHomePoint(m_stHomePoint);
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
        break;
    case View_3DMap:
        if(!m_p3DEarthManipulator.valid())
        {
            m_p3DEarthManipulator = new CMyEarthManipulator(MAP_3D);
            m_p3DEarthManipulator->InitHomePoint(m_stHomePoint);
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
        break;
    case View_Osg:
        if(!m_pSelfManipulator.valid())
        {
            m_pSelfManipulator = new osgGA::TrackballManipulator;
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
        break;
    case View_Node:
        break;
    }

    m_emType = emType;
}

/// 设置跟踪视点
void QtViewPort::TrackNodeChanged()
{
    if(m_pSceneGraph != m_pTrackNode->GetBoundSceneGraph())
    {
        return;
    }

    IOsgSceneNode* pOsgNode = m_pTrackNode->AsOsgSceneNode();

    if(pOsgNode)
    {
        if(!m_pTrackManipulator.valid())
        {
            m_pTrackManipulator = new osgGA::NodeTrackerManipulator;
            m_pTrackManipulator->setDistance(m_stViewPoint.fDistance);

            m_emPreType = m_emType;
            m_emType = View_Node;

            m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pTrackManipulator));
        }
        else
        {
            m_pTrackManipulator->setRotationMode(osgGA::NodeTrackerManipulator::ELEVATION_AZIM);
            m_emPreType = m_emType;
            m_emType = View_Node;
            m_pTrackManipulator->setTrackNode(pOsgNode->GetOsgNode());
        }
    }
    else
    {
        switch (m_emPreType)
        {
        case View_3DMap:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
            break;
        case View_2DMap:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
            break;
        default:
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
            break;
        }
    }
}

/// 视口更改
void QtViewPort::ViewPortChanged()
{
    m_pCameraUpdate->UpdateViewPort();
}

void QtViewPort::ProjectTypeChanged()
{
    m_pCameraUpdate->UpdateProject();
}

/// 获取屏显节点
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
    m_stViewPoint = rViewPoint;
    m_dTimes = unTimes;
    m_bViewPointChanged=true;
}

/// 获取视点位置
const SceneViewPoint &QtViewPort::GetViewPoint() const
{
    return(m_stViewPoint);
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

    /// home视点更改
    if(m_bHomePointChanged)
    {
        switch(m_emType)
        {
        case View_2DMap:
            if(m_p2DEarthManipulator.valid())
            {
                m_p2DEarthManipulator->InitHomePoint(m_stHomePoint);
            }
            break;
        case View_3DMap:
            if(m_p3DEarthManipulator)
            {
                m_p3DEarthManipulator->InitHomePoint(m_stHomePoint);
            }
            break;
        default:
            break;
        }

        m_bHomePointChanged=false;
    }

    if(m_bViewPointChanged)
    {
        switch(m_emType)
        {
        case View_3DMap:
            m_p3DEarthManipulator->SetViewPoint(m_stViewPoint,m_dTimes);
            break;
        case View_2DMap:
            m_p2DEarthManipulator->SetViewPoint(m_stViewPoint,m_dTimes);
            break;
        case View_Node:
            m_pTrackManipulator->setDistance(m_stViewPoint.fDistance);
            m_pTrackManipulator->setHeading(osg::DegreesToRadians(m_stViewPoint.fAzimuth));
            m_pTrackManipulator->setElevation(osg::DegreesToRadians(m_stViewPoint.fElev));
            break;
        case View_Osg:
            m_pSelfManipulator->setDistance(m_stViewPoint.fDistance);
            m_pSelfManipulator->setCenter(osg::Vec3(m_stViewPoint.stPos.fX,m_stViewPoint.stPos.fY,
                                                    m_stViewPoint.stPos.fZ));
            m_pSelfManipulator->setHeading(osg::DegreesToRadians(m_stViewPoint.fAzimuth));
            m_pSelfManipulator->setElevation(osg::DegreesToRadians(m_stViewPoint.fElev));
            break;
        default:
            break;
        }
        m_bViewPointChanged=false;
    }
}

void QtViewPort::HomeViewPoint()
{
    switch(m_emType)
    {
    case View_3DMap:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_p3DEarthManipulator));
        break;
    case View_2DMap:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_p2DEarthManipulator));
        break;
    case View_Node:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_pTrackManipulator));
        break;
    case View_Osg:
        m_pRender->AddUpdateOperation(new HomeViewPointOperation(m_pSelfManipulator));
        break;
    default:
        break;
    }
}

bool QtViewPort::SubMessage(IViewPortMessageObserver *pObserver)
{
    auto findResult = find(m_pAllOserver.begin(),m_pAllOserver.end(),pObserver);
    if(findResult == m_pAllOserver.end())
    {
        m_pAllOserver.push_back(pObserver);
    }

    return(true);
}

bool QtViewPort::UnSubMessage(IViewPortMessageObserver *pObserver)
{
    auto findResult = find(m_pAllOserver.begin(),m_pAllOserver.end(),pObserver);
    if(findResult != m_pAllOserver.end())
    {
        m_pAllOserver.erase(findResult);
        return(true);
    }
    else
    {
        return(false);
    }
}

/// 视点位置
void QtViewPort::EyePos(double dX, double dY, double dZ)
{
    static ScenePos s_stWordPos;
    auto iter = m_pAllOserver.begin();
    auto tmpIter = iter;

    s_stWordPos.fX = dX;
    s_stWordPos.fY = dY;
    s_stWordPos.fZ = dZ;

    for(;iter != m_pAllOserver.end();)
    {
        ++tmpIter;
        (*iter)->EypePos(s_stWordPos);
        iter = tmpIter;
    }
}

/// 查看的位置
void QtViewPort::LookDir(double dX, double dY, double dZ)
{
    static ScenePos s_stWordPos;
    auto iter = m_pAllOserver.begin();
    auto tmpIter = iter;

    s_stWordPos.fX = dX;
    s_stWordPos.fY = dY;
    s_stWordPos.fZ = dZ;

    for(;iter != m_pAllOserver.end();)
    {
        ++tmpIter;
        (*iter)->LookDir(s_stWordPos);
        iter = tmpIter;
    }
}

/// 移除从相机
void QtViewPort::RemoveSlave()
{
    if(m_listStereoCamera.size() > 0)
    {
        m_pView->getCamera()->setGraphicsContext(m_listStereoCamera.front()->getGraphicsContext());
        for(auto one : m_listStereoCamera)
        {
            m_pView->removeSlave(m_pView->findSlaveIndexForCamera(one));
        }
        m_listStereoCamera.clear();
    }
}
