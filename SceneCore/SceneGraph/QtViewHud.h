#ifndef QT_VIEWHUD_H
#define QT_VIEWHUD_H
#include <osg/View>
#include <osgEarth/Controls>
#include "SceneGraph/IViewHud.h"
class QtViewHud:public IViewHud
{
public:
    QtViewHud(osg::View*);
    ~QtViewHud();
private:
    osg::observer_ptr<osg::View> m_pView;
    osg::ref_ptr<osgEarth::Controls::ControlCanvas> m_pControlCanvas;
};

#endif // QTVIEWHUD_H
