#include <Inner/ILoadResource.h>
#include "HudButton.h"

struct ClickViewpointHandler : public osgEarth::Util::Controls::ControlEventHandler
{
    ClickViewpointHandler(CHudButton *bt)
    {
        m_hudButton = bt;
    }
private:
    CHudButton * m_hudButton = nullptr;
    virtual void onClick(osgEarth::Util::Controls::Control* control)
    {
        if(m_hudButton)
        {
            for(auto index = m_hudButton->m_pAllButtonObserver.begin();index != m_hudButton->m_pAllButtonObserver.end();++index)
            {
                (*index)->ButtonOnClick(m_hudButton);
            }
        }
    }
};


///初始化hud字体
bool CHudButton::SubObserver(IHudButtonObserver *pObserver)
{
    if (nullptr == pObserver)
    {
        return(false);
    }

    auto findItor = std::find(m_pAllButtonObserver.begin(),m_pAllButtonObserver.end(),pObserver);
    if (findItor == m_pAllButtonObserver.end())
    {
        m_pAllButtonObserver.push_back(pObserver);
        return(true);
    }
    return(false);

}

bool CHudButton::UnSubObserver(IHudButtonObserver *pObserver)
{
    if (nullptr == pObserver)
    {
        return(false);
    }

    auto findItor = std::find(m_pAllButtonObserver.begin(),m_pAllButtonObserver.end(),pObserver);
    if (findItor == m_pAllButtonObserver.end())
    {
        return(false);
    }

    m_pAllButtonObserver.erase(findItor);
    return(true);
}

void CHudButton::InitNode()
{
    ImplHudNode<IHudButton>::InitNode();
    m_stColor.fR  = 1.f;
    m_pButtonControl = new osgEarth::Controls::ButtonControl;
    m_pButtonControl->setEncoding(osgText::String::ENCODING_UTF8);

    m_pButtonControl->setForeColor(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA);
    m_pButtonControl->setFontSize(m_nFontSize);
    m_pButtonControl->setHaloColor(osg::Vec4(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA));
    m_pButtonControl->addEventHandler( new ClickViewpointHandler(this) );

    SetControlNode(m_pButtonControl.get());
}

/// 更新节点
void CHudButton::FrameCall()
{
    if(m_bFontChanged)
    {
        m_pButtonControl->setFont(m_pSceneGraph->ResouceLoader()->LoadFont(m_sFont));
        m_bFontChanged=false;
    }

    if(m_bFontSizeChanged)
    {
        m_pButtonControl->setFontSize(m_nFontSize);
        m_bFontSizeChanged=false;
    }

    if(m_bTextChanged)
    {
        m_pButtonControl->setText(m_sText);
        m_bTextChanged=false;
    }

    if(m_bColorChanged)
    {
        m_pButtonControl->setForeColor(osg::Vec4(m_stColor.fR,m_stColor.fG,m_stColor.fB,m_stColor.fA));
        m_bColorChanged=false;
    }

    if(m_bOutColorChanged)
    {
        m_pButtonControl->setHaloColor(osg::Vec4(m_stOutLineColor.fR,m_stOutLineColor.fG,m_stOutLineColor.fB,m_stOutLineColor.fA));
        m_bOutColorChanged=false;
    }

    ImplHudNode<IHudButton>::FrameCall();
}
