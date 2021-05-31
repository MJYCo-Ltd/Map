#ifndef CHUDTEXT_H
#define CHUDTEXT_H
#include <Plot/Hud/IHudText.h>
#include <Inner/ImplHudNode.hpp>

class CHudText:public ImplHudNode<IHudText>
{
public:
    CONSTRUCTOR(CHudText,ImplHudNode<IHudText>)
protected:
    void InitNode();
    void UpdateNode();
    void FontSizeChanged()SET_TRUE_NODE_UPDATE(m_bFontSizeChanged)
    void TextChanged()SET_TRUE_NODE_UPDATE(m_bTextChanged)
    void FontChanged()SET_TRUE_NODE_UPDATE(m_bFontChanged)
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void OutColorChanged()SET_TRUE_NODE_UPDATE(m_bOutColorChanged)
protected:
    osg::observer_ptr<osgEarth::Controls::LabelControl> m_pLabelControl;
    bool m_bFontSizeChanged{false};
    bool m_bTextChanged{false};
    bool m_bFontChanged{false};
    bool m_bColorChanged{false};
    bool m_bOutColorChanged{false};
};

#endif // CHUDTEXT_H
