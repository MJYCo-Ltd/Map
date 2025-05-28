#ifndef CHUDBUTTON_H
#define CHUDBUTTON_H
#include <Plot/Hud/IHudButton.h>
#include <Inner/ImplHudNode.hpp>

class CHudButton:public ImplHudNode<IHudButton>
{
    friend struct ClickViewpointHandler;
public:
    CONSTRUCTOR(CHudButton,ImplHudNode<IHudButton>)
    virtual bool SubObserver(IHudButtonObserver* pObserver);
    virtual bool UnSubObserver(IHudButtonObserver* pObserver);
protected:
    void InitNode();
    void FrameCall();
    void FontSizeChanged()SET_TRUE_NODE_UPDATE(m_bFontSizeChanged)
    void TextChanged()SET_TRUE_NODE_UPDATE(m_bTextChanged)
    void FontChanged()SET_TRUE_NODE_UPDATE(m_bFontChanged)
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void OutColorChanged()SET_TRUE_NODE_UPDATE(m_bOutColorChanged)
protected:
    osg::observer_ptr<osgEarth::Controls::ButtonControl> m_pButtonControl;
    bool m_bFontSizeChanged{false};
    bool m_bTextChanged{false};
    bool m_bFontChanged{false};
    bool m_bColorChanged{false};
    bool m_bOutColorChanged{false};
    std::list<IHudButtonObserver*> m_pAllButtonObserver;
};

#endif // CHUDBUTTON_H
