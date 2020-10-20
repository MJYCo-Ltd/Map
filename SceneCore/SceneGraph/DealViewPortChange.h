#ifndef DEALVIEWPORTCHANGE_H
#define DEALVIEWPORTCHANGE_H

#include <osg/OperationThread>

/// 视点更新回调
class QtViewPointUpdateCallback:public osg::Callback
{
public:

    QtViewPointUpdateCallback(QtViewPort* pViewPoint):m_pViewPoint(pViewPoint){}

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
    QtViewPort* m_pViewPoint;
    bool         m_bUpdateViewPort=false;
    bool         m_bUpdateProject=false;
};

///  更改操作器
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

#endif // DEALVIEWPORTCHANGE_H
