#include <osgEarth/GeoData>
#include <osg/OperationThread>
#include <osgViewer/ViewerEventHandlers>

#include <Inner/QtSceneNode.h>

#include "MyEarthManipulator.h"
#include "QtViewPoint.h"

class QtViewPointUpdateCallback:public osg::Callback
{
public:

    QtViewPointUpdateCallback(QtViewPoint* pViewPoint):m_pViewPoint(pViewPoint){}

    /// 更新视口
    void UpdateViewPort()
    {
        m_bUpdateViewPort = true;
    }

    void UpdateProject()
    {
        m_bUpdateProject = true;
    }

    bool run(osg::Object* object, osg::Object* data)
    {
        ///  更新视口
        if(m_bUpdateViewPort)
        {
            auto pCamera = static_cast<osg::Camera*>(object);
            if(nullptr != pCamera)
            {
                const CameraViewPort& rViewPort = m_pViewPoint->GetViewPort();
                pCamera->setViewport(rViewPort.nX,rViewPort.nY,rViewPort.nWidth,rViewPort.nHeight);
                m_bUpdateViewPort = false;
            }
        }

        /// 更新投影方式
        if(m_bUpdateProject)
        {
            auto pCamera = static_cast<osg::Camera*>(object);
            if(nullptr != pCamera)
            {
                const CameraViewPort& rViewPort = m_pViewPoint->GetViewPort();

                double dAcesip = static_cast<double>(rViewPort.nWidth)/static_cast<double>(rViewPort.nHeight);
                switch (m_pViewPoint->GetProjectType())
                {
                case Ortho:
                    pCamera->setProjectionMatrixAsOrtho2D(rViewPort.nX,rViewPort.nWidth,rViewPort.nY,rViewPort.nHeight);
                    break;
                case Perspective:
                    pCamera->setProjectionMatrixAsPerspective(45,dAcesip,0.01,100.);
                    break;
                }
                m_bUpdateProject = false;
            }
        }

        return osg::Callback::run(object, data);
    }

private:
    QtViewPoint* m_pViewPoint;
    bool         m_bUpdateViewPort=false;
    bool         m_bUpdateProject=false;
};

#include <QDebug>
class ChangeManipulator:public osg::Operation
{
public:
    ChangeManipulator(osgViewer::View* pView,osgGA::CameraManipulator* pManipulator):
        m_pView(pView),
        m_pManipulator(pManipulator){}

    void operator()(osg::Object*)
    {
        m_pView->setCameraManipulator(m_pManipulator);
    }

private:
    osg::ref_ptr<osgViewer::View> m_pView;
    osg::ref_ptr<osgGA::CameraManipulator> m_pManipulator;
};

/// 视点
QtViewPoint::QtViewPoint(IRender *pRender, ProjectType emProject):
    m_pRender(pRender),
    m_emProjectType(emProject)
{
    m_pView = new osgViewer::View;
    m_pView->addEventHandler(new osgViewer::StatsHandler);

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

    m_pSelfManipulator = new osgGA::TrackballManipulator;
    m_pView->setCameraManipulator(m_pSelfManipulator);
}

/// 析构函数
QtViewPoint::~QtViewPoint()
{
    m_pView->getCamera()->removeUpdateCallback(m_pCameraUpdate);
}

/// 地图类型切换
void QtViewPoint::MapTypeChanged(MapType emType)
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

        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
    }
        break;
    case MAP_3D:
        if(!m_p3DEarthManipulator.valid())
        {
            m_p3DEarthManipulator = new CMyEarthManipulator(emType);
            m_p3DEarthManipulator->InitViewPoint();
        }
        m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p3DEarthManipulator));
        break;
    }
}

/// 设置跟踪视点
void QtViewPoint::SetTrackNode(ISceneNode *pTrackNode)
{
    if(!m_pTrackManipulator.valid())
    {
        m_pTrackManipulator = new osgGA::NodeTrackerManipulator;

        if(nullptr != pTrackNode)
        {
            m_pTrackNode = pTrackNode;

            m_pTrackManipulator->setTrackNode(m_pTrackNode->GetOsgNode());
            m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pTrackManipulator));
        }
    }
    else
    {
        if(nullptr != pTrackNode && pTrackNode != m_pTrackNode)
        {
            m_pTrackNode = pTrackNode;
            m_pTrackManipulator->setTrackNode(m_pTrackNode->GetOsgNode());
        }
        else if(nullptr == pTrackNode)
        {
            if(m_bIsMap)
            {
                m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_p2DEarthManipulator));
            }
            else
            {
                m_pRender->AddUpdateOperation(new ChangeManipulator(m_pView,m_pSelfManipulator));
            }
        }
    }
}

ISceneNode *QtViewPoint::GetTrackNode()
{
    return(m_pTrackNode);
}

/// 设置视点
void QtViewPoint::SetViewPoint(const SceneViewPoint & rViewPoint)
{
    if(rViewPoint != m_rViewPoint)
    {
        m_rViewPoint = rViewPoint;
    }
}

/// 获取视点位置
const SceneViewPoint &QtViewPoint::GetViewPoint()
{
    return(m_rViewPoint);
}

/// 设置视口
void QtViewPoint::SetViewPort(const CameraViewPort &rViewPort)
{
    if(m_rViewPort != rViewPort)
    {
        m_rViewPort = rViewPort;

        m_pCameraUpdate->UpdateViewPort();
    }
}

/// 获取窗口大小
const CameraViewPort &QtViewPoint::GetViewPort()
{
    return(m_rViewPort);
}

/// 设置投影类型
void QtViewPoint::SetProjectType(ProjectType emProject)
{
    if(emProject != m_emProjectType)
    {
        m_emProjectType = emProject;
        m_pCameraUpdate->UpdateProject();
    }
}

/// 返回投影类型
ProjectType QtViewPoint::GetProjectType()
{
    return(m_emProjectType);
}

/// 获取视图类
osgViewer::View *QtViewPoint::GetOsgView()
{
    return(m_pView.get());
}
