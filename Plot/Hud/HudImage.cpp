#include "HudImage.h"
#include<osgEarth/Units>
void CHudImage::InitNode()
{
    ImplHudNode<IHudImage>::InitNode();
    m_pImageControl = new osgEarth::Controls::ImageControl;
    m_pImageControl->setRotation(osgEarth::Angular(m_dAngle, osgEarth::Units::DEGREES));
    m_pImageControl->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
    m_pImageControl->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
    m_pImageControl->setFixSizeForRotation(true);

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

            m_pImageControl->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
            m_pImageControl->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        }
        m_bImageChanged=false;
    }
    if(m_bAngleChanged)
    {
        m_pImageControl->setRotation(osgEarth::Angular(m_dAngle, osgEarth::Units::DEGREES));
        m_bAngleChanged=false;
    }

    if(m_bImageSizeChanged)
    {
        m_pImageControl->setSize(m_stImageSize.unWidth,m_stImageSize.unHeight);
        m_bImageSizeChanged = false;
    }
    ImplHudNode<IHudImage>::FrameCall();
}
