#ifndef VIEWPORTEVENTCALLBACK_H
#define VIEWPORTEVENTCALLBACK_H
#include "QtViewPort.h"

/**
 * @brief The ViewPortEventCallback class
 */
class ViewPortEventCallback:public osgGA::GUIEventHandler
{
public:
    ViewPortEventCallback(QtViewPort* pViewPort):m_pViewPort(pViewPort){}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa,
                        osg::Object*, osg::NodeVisitor*)
    {
        if(ea.FRAME == ea.getEventType())
        {
            /// 每一帧更新一次
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
    QtViewPort* m_pViewPort{};
    osg::Vec3d  m_vEye;
    osg::Vec3d  m_vCenter;
    osg::Vec3d  m_vUp;
};

#endif // VIEWPORTEVENTCALLBACK_H
