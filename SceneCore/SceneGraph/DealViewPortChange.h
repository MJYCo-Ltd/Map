#ifndef DEALVIEWPORTCHANGE_H
#define DEALVIEWPORTCHANGE_H

#include <osg/OperationThread>
#include "QtViewPort.h"

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

/// 更改视点
class ChangeViewPoint:public osg::Operation
{
public:
    ChangeViewPoint(osgGA::CameraManipulator* pManipulator,SceneViewPoint viewPoint,IOsgViewPoint::ViewPointType type):
        m_pManipulator(pManipulator),
        m_stViewPoint(viewPoint),
        m_emPointType(type){}

    void operator()(osg::Object*)
    {
        m_pManipulator;
    }
private:
    osg::ref_ptr<osgGA::CameraManipulator> m_pManipulator;
    SceneViewPoint m_stViewPoint;
    IOsgViewPoint::ViewPointType  m_emPointType;
};

/// 更改视点
class HomeViewPointOperation:public osg::Operation
{
public:
    HomeViewPointOperation(osgGA::CameraManipulator* pManipulator):m_pManipulator(pManipulator){}

    void operator()(osg::Object*)
    {
        m_pManipulator->home(0);
    }
private:
    osg::ref_ptr<osgGA::CameraManipulator> m_pManipulator;
};

#endif // DEALVIEWPORTCHANGE_H
