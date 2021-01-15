#include "HudImage.h"

void CHudImage::InitNode()
{
    ImplHudNode<IHudImage>::InitNode();
    m_pImageControl = new osgEarth::Controls::ImageControl;

    m_pImageControl->setAlign(osgEarth::Controls::Control::ALIGN_LEFT,osgEarth::Controls::Control::ALIGN_BOTTOM);

    SetControlNode(m_pImageControl.get());
}

void CHudImage::UpdateNode()
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
    ImplHudNode<IHudImage>::UpdateNode();
}
