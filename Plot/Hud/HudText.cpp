#include <Inner/ILoadResource.h>
#include "HudText.h"

///初始化hud字体
void CHudText::InitNode()
{
    ImplHudNode<IHudText>::InitNode();
    m_stColor.fR  = 1.f;
    m_pLabelControl = new osgEarth::Controls::LabelControl;
    m_pLabelControl->setEncoding(osgText::String::ENCODING_UTF8);

    m_pLabelControl->setForeColor(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    m_pLabelControl->setFontSize(m_nFontSize);
    m_pLabelControl->setHaloColor(osg::Vec4(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA));
    m_pLabelControl->setAlign(osgEarth::Controls::Control::ALIGN_LEFT,osgEarth::Controls::Control::ALIGN_BOTTOM);

    SetControlNode(m_pLabelControl.get());
}

/// 更新节点
void CHudText::UpdateNode()
{
    if(m_bFontChanged)
    {
        m_pLabelControl->setFont(m_pSceneGraph->ResouceLoader()->LoadFont(m_sFont));
        m_bFontChanged=false;
    }

    if(m_bFontSizeChanged)
    {
        m_pLabelControl->setFontSize(m_nFontSize);
        m_bFontSizeChanged=false;
    }

    if(m_bTextChanged)
    {
        m_pLabelControl->setText(m_sText);
        m_bTextChanged=false;
    }

    if(m_bColorChanged)
    {
        m_pLabelControl->setForeColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
        m_bColorChanged=false;
    }

    if(m_bOutColorChanged)
    {
        m_pLabelControl->setHaloColor(osg::Vec4(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA));
        m_bOutColorChanged=false;
    }

    ImplHudNode<IHudText>::UpdateNode();
}
