#ifndef DEALVIEWPORTCHANGE_H
#define DEALVIEWPORTCHANGE_H

#include <osg/OperationThread>
#include "QtViewPort.h"

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
