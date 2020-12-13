#ifndef IMPL_HUD_NODE_H
#define IMPL_HUD_NODE_H

#include <osgEarth/Controls>
#include <Inner/ImplSceneNode.hpp>
/**
 *  实现IHudNode所有的接口
 */
template <typename T>
class ImplHudNode:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplHudNode,ImplSceneNode<T>)

protected:
    void SetControlNode(osgEarth::Controls::Control* pControl)
    {
        m_pControl = pControl;
        SetOsgNode(pControl);
    }

    /// 位置更改
    void HudPosChanged()
    {
        m_bPosChanged = true;
        ImplSceneNode<T>::NodeChanged();
    }

    /// 状态更改
    void HudTypeChanged()
    {
        m_bTypeChanged=true;
        ImplSceneNode<T>::NodeChanged();
    }

    void UpdateNode()
    {
        if(m_bPosChanged)
        {
            m_pControl->setPosition(T::m_stHudPos.nX,T::m_stHudPos.nY);
            m_bPosChanged = false;
        }

        if(m_bTypeChanged)
        {
            switch (T::m_emHudPosType)
            {
            case HUD_UP_CENTER:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_CENTER,osgEarth::Controls::Control::ALIGN_TOP);
                break;
            case HUD_DOWN_CENTER:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_CENTER,osgEarth::Controls::Control::ALIGN_BOTTOM);
                break;
            case HUD_UP_RIGHT:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_RIGHT,osgEarth::Controls::Control::ALIGN_TOP);
                break;
            case HUD_DOWN_RIGHT:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_RIGHT,osgEarth::Controls::Control::ALIGN_BOTTOM);
                break;
            case HUD_UP_LEFT:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_LEFT,osgEarth::Controls::Control::ALIGN_TOP);
                break;
            case HUD_DOWN_LEFT:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_LEFT,osgEarth::Controls::Control::ALIGN_BOTTOM);
                break;
            case HUD_FIXED:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_NONE,osgEarth::Controls::Control::ALIGN_NONE);
                break;
            case HUD_RIGHT_CENTER:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_RIGHT,osgEarth::Controls::Control::ALIGN_CENTER);
                break;
            case HUD_LEFT_CENTER:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_LEFT,osgEarth::Controls::Control::ALIGN_CENTER);
                break;
            case HUD_CENTER_CENTER:
                m_pControl->setAlign(osgEarth::Controls::Control::ALIGN_CENTER,osgEarth::Controls::Control::ALIGN_CENTER);
                break;
            }
            m_bTypeChanged = false;
        }
        ImplSceneNode<T>::UpdateNode();
    }
protected:
    osg::observer_ptr<osgEarth::Controls::Control> m_pControl;
    bool m_bPosChanged=false;
    bool m_bTypeChanged=false;
};

#endif // IMPL_HUD_NODE_H