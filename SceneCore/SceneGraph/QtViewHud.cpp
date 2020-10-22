#include <Inner/ILoadResource.h>
#include <osgText/Font>
#include "QtViewHud.h"
QtViewHud::QtViewHud(osg::View *pView, ISceneGraph *pSceneGraph)
    :ImplSceneNode<IViewHud>(pSceneGraph),m_pView(pView)
{
}

QtViewHud::~QtViewHud()
{
}

/// 添加屏显节点
bool QtViewHud::AddHudNode(IHudNode *pHudNode)
{
    if(m_pSceneGraph != pHudNode->GetBoundSceneGraph() ||
      m_setHudNode.find(pHudNode) != m_setHudNode.end())
    {
        return(false);
    }
    return(true);
}

bool QtViewHud::RemoveHudNode(IHudNode *pHudNode)
{
    return(true);
}

void QtViewHud::Clear()
{
}
