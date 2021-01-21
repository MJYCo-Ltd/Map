#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IWindow.h>
#include <SceneGraph/IViewPort.h>
#include <osgDB/WriteFile>
#include <Inner/IOsgViewPoint.h>
#include <Inner/IOsgSceneNode.h>
#include "PointPick.h"

struct RttPickerCallBack:public osgEarth::Util::RTTPicker::Callback
{
    /**
     * @brief 获取到pick到的目标ID
     */
    virtual void onHit(osgEarth::ObjectID id)
    {
        osg::notify(osg::WARN)<<"pick id:"<<id<<std::endl;
    }

    /**
     * @brief 没有pick到目标
     */
    virtual void onMiss()
    {
        osg::notify(osg::WARN)<<"Miss:"<<std::endl;
    }

    /**
     * @brief 是否接收消息
     */
    virtual bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
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

    osgEarth::Util::RTTPicker* m_pPicker;
};

void CPointPick::InitTool()
{
    m_pPicker = new osgEarth::Util::RTTPicker;
    auto pView = dynamic_cast<IOsgViewPoint*>(m_pSceneGraph->GetMainWindow()->GetMainViewPoint());
    if(nullptr != pView)
    {
        pView->GetOsgView()->addEventHandler(m_pPicker);
//        osg::Texture2D* pTexure2D = m_pPicker->getOrCreateTexture(pView->GetOsgView());
        m_pPicker->setDefaultCallback(new RttPickerCallBack);
    }

    auto pOsgNode = dynamic_cast<IOsgSceneNode*>(m_pSceneGraph->GetRoot());
    {
        if(nullptr != pOsgNode)
        {
            m_pPicker->addChild(pOsgNode->GetOsgNode());
        }
    }
}

void CPointPick::ReleaseTool()
{
}
