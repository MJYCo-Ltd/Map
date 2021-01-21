#include "HudLayout.h"

bool CHudLayout::AddHudNode(IHudNode *pNode)
{
    if (nullptr == pNode)
    {
        return(false);
    }

    /// 如果两者不是同一个渲染器,则返回
    if (pNode->GetBoundSceneGraph() != m_pSceneGraph)
    {
        return(false);
    }

    IOsgSceneNode* pOsgSceneNode = pNode->AsOsgSceneNode();

    auto findItor = std::find(m_pAllChild.begin(),m_pAllChild.end(),pOsgSceneNode);
    if (findItor == m_pAllChild.end())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControl2Container(
                        m_pLayout.get(),
                        dynamic_cast<osgEarth::Controls::Control*>(pOsgSceneNode->GetOsgNode())));
        m_pAllChild.push_back(pOsgSceneNode);
        return(true);
    }

    return(true);
}

bool CHudLayout::RemoveHudNode(IHudNode *pNode)
{
    if (nullptr == pNode)
    {
        return(false);
    }

    IOsgSceneNode* pOsgSceneNode = pNode->AsOsgSceneNode();
    auto findItor = std::find(m_pAllChild.begin(),m_pAllChild.end(),pOsgSceneNode);
    if (findItor != m_pAllChild.end())
    {
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CClearContainer(
                        m_pLayout.get()));

        osgEarth::Controls::ControlVector vControl;
        for(auto one : m_pAllChild)
        {
            if(one != pOsgSceneNode)
            {
                vControl.push_back(dynamic_cast<osgEarth::Controls::Control*>(one->GetOsgNode()));
            }
        }
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControls2Container(
                        m_pLayout.get(),
                        vControl));

        pNode->HudTypeChanged();
    }

    return(true);
}

void CHudLayout::InitNode()
{
    ImplHudNode<IHudLayout>::InitNode();
    m_pLayout = new osgEarth::Controls::HBox;
    SetControlNode(m_pLayout.get());
}

/// 更新节点
void CHudLayout::UpdateNode()
{
    if(m_bLayoutChanged)
    {
        osgEarth::Controls::Container* pContainer=nullptr;
        if(LAYOUT_VERTICAL == m_emType)
        {
            pContainer = new osgEarth::Controls::VBox;
        }
        else
        {
            pContainer = new osgEarth::Controls::HBox;
        }

        osgEarth::Controls::ControlVector vControl;
        for(auto one : m_pAllChild)
        {
            vControl.push_back(dynamic_cast<osgEarth::Controls::Control*>(one->GetOsgNode()));
        }
        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new CAddControls2Container(
                        pContainer,
                        vControl));

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        m_pLayout.get(),
                        pContainer,true));

        m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                    new COSGAttachNode(
                        m_pLayout.get(),
                        m_pLayout.get(),false));

        m_pLayout = pContainer;
        m_bLayoutChanged = false;

        SetControlNode(m_pLayout.get());
        m_bPosChanged=true;
        m_bTypeChanged=true;
        NodeChanged();
    }
    else
    {
        ImplHudNode<IHudLayout>::UpdateNode();
    }
}
