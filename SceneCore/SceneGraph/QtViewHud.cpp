#include <osgEarth/NodeUtils>
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

    m_setHudNode.insert(pHudNode);

    AddControl(m_pControlCanvas.get(),dynamic_cast<osgEarth::Controls::Control*>(pHudNode->AsOsgSceneNode()->GetOsgNode()));
    return(true);
}

/// 移除节点
bool QtViewHud::RemoveHudNode(IHudNode *pHudNode)
{
    auto findOne = m_setHudNode.find(pHudNode);
    if(findOne == m_setHudNode.end())
    {
        return(false);
    }

    DelControl(m_pControlCanvas.get(),dynamic_cast<osgEarth::Controls::Control*>(pHudNode->AsOsgSceneNode()->GetOsgNode()));
    m_setHudNode.erase(pHudNode);
    return(true);
}

/// 清空所有的
void QtViewHud::Clear()
{
    for(auto one : m_setHudNode)
    {
        DelControl(m_pControlCanvas.get(),dynamic_cast<osgEarth::Controls::Control*>(one->AsOsgSceneNode()->GetOsgNode()));
    }

    m_setHudNode.clear();
}

/// 初始化节点
void QtViewHud::InitNode()
{
    ImplSceneNode<IViewHud>::InitNode();

    osgEarth::Controls::ControlCanvas* canvas = osgEarth::findTopMostNodeOfType<osgEarth::Controls::ControlCanvas>(m_pView->getCamera());
    if ( canvas )
    {
        m_pControlCanvas = canvas;
    }
    else
    {
        m_pControlCanvas = new osgEarth::Controls::ControlCanvas();
    }

    ImplSceneNode<IViewHud>::SetOsgNode(m_pControlCanvas.get());
}

void QtViewHud::AddControl(osgEarth::Util::Controls::ControlCanvas *pCanvas, osgEarth::Util::Controls::Control *pControl)
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyControl(pCanvas,pControl,true));
}

void QtViewHud::DelControl(osgEarth::Util::Controls::ControlCanvas *pCanvas, osgEarth::Util::Controls::Control *pControl)
{
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CModifyControl(pCanvas,pControl,false));
}
