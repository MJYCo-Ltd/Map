#include "HudImage.h"

void CHudImage::InitNode()
{
    ImplHudNode<IHudImage>::InitNode();
    m_pImageControl = new osgEarth::Controls::ImageControl;

    SetControlNode(m_pImageControl.get());
}

void CHudImage::FrameCall()
{
    if(m_bImageChanged)
    {
        auto pTexture = m_pSceneGraph->ResouceLoader()->LoadTexture(m_sImagePath);
        if(nullptr != pTexture)
        {
            m_pImageControl->setTexture(pTexture);
        }
        m_bImageChanged=false;
    }
    ImplHudNode<IHudImage>::FrameCall();
}
