#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <Inner/IOsgViewPoint.h>
#include <Inner/IOsgSceneNode.h>
#include <Inner/IRender.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include "../ToolSelector/ToolSelector.h"
#include "PointPick.h"

struct RttPickerCallBack:public osgEarth::Util::RTTPicker::Callback
{
    RttPickerCallBack(CPointPick*pPointPick):m_pPicker(pPointPick){}
    /**
     * @brief 获取到pick到的目标ID
     */
    virtual void onHit(osgEarth::ObjectID id)
    {
        QMetaObject::invokeMethod(m_pPicker,"HitId",Q_ARG(unsigned int,id));
    }

    /**
     * @brief 没有pick到目标
     */
    virtual void onMiss()
    {
        QMetaObject::invokeMethod(m_pPicker,"HitId",Q_ARG(unsigned int,0));
    }

    /**
     * @brief 是否接收消息
     */
    virtual bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter&)
    {
        if(ea.getButton()==ea.LEFT_MOUSE_BUTTON && ea.getEventType() == ea.PUSH)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    CPointPick* m_pPicker;
};


CPointPick::CPointPick(ISceneGraph *pSceneGraph, CToolSelector *pToolSelector):
    IToolBase(pSceneGraph,pToolSelector){}

/// 初始化工具
bool CPointPick::InitTool()
{
    auto pView = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    auto pOsgNode = dynamic_cast<IOsgSceneNode*>(m_pSceneGraph->GetRoot());
    if(nullptr == pView || nullptr == pOsgNode) return(false);

    m_pPicker = new osgEarth::Util::RTTPicker;
    m_pPicker->setCullMask(PICK_MASK);

    m_pPicker->setDefaultCallback(new RttPickerCallBack(this));
    m_pPicker->addChild(pOsgNode->GetOsgNode());
    m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                new CModifyViewHandler(pView->GetOsgView(),m_pPicker.get(),true));

    return(true);
}

/// 释放工具
void CPointPick::ReleaseTool()
{
    auto pView = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    if(nullptr != pView)
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CModifyViewHandler(pView->GetOsgView(),m_pPicker.get(),false));
    }
}

void CPointPick::HitId(unsigned int unID)
{
    m_pToolSelector->PickID(unID);
}
