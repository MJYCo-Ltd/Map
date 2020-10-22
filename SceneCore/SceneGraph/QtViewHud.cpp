#include <Inner/ILoadResource.h>
#include <osgText/Font>
#include "QtViewHud.h"
QtViewHud::QtViewHud(osg::View *pView, ISceneGraph *pSceneGraph)
    :IViewHud(pSceneGraph),m_pView(pView)
{
    m_pControlCanvas = osgEarth::Controls::ControlCanvas::getOrCreate(m_pView.get());
}

QtViewHud::~QtViewHud()
{
}

void QtViewHud::AddHudNode(IHudNode *pHudNode)
{
}

void QtViewHud::RemoveHudNode(IHudNode *pHudNode)
{
}

void QtViewHud::Clear()
{
}
