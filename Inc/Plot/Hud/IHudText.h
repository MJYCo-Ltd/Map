#ifndef INTERFACE_HUD_TEXT_HEARDER_H
#define INTERFACE_HUD_TEXT_HEARDER_H

#include <string>
#include <Hud/IHudNode.h>

/**
 * @brief 视口的屏显类
 */
class IHudText:public IHudNode
{
public:
    CONSTRUCTOR(IHudText,IHudNode)

    /**
     * @brief 设置显示内容
     * @attention utf-8编码方式
     */
    void SetText(const std::string& sText)JUDGE_EQUAL_CALL_FUNCTION(sText,m_sText,TextChanged)
    const std::string& Text()const{return(m_sText);}

    /**
     * @brief 设置字体
     */
    void SetFont(const std::string& sFont)JUDGE_EQUAL_CALL_FUNCTION(sFont,m_sFont,FontChanged)
    const std::string& Font()const{return(m_sFont);}

    /**
     * @brief 设置字体填充颜色
     */
    void SetColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stColor,ColorChanged)
    const SceneColor& Color()const{return(m_stColor);}

    /**
     * @brief 设置字体边框颜色
     */
    void SetOutColor(const SceneColor& rColor)JUDGE_EQUAL_CALL_FUNCTION(rColor,m_stOutLineColor,OutColorChanged)
    const SceneColor& OutLineColor()const{return(m_stOutLineColor);}

    /**
     * @brief 设置字体大小
     */
    void SetFontSize(int nFontSize)JUDGE_EQUAL_CALL_FUNCTION(nFontSize,m_nFontSize,FontSizeChanged)
    int  FontSize() const{return(m_nFontSize);}
protected:
    virtual ~IHudText(){}

    virtual void FontSizeChanged()=0;
    virtual void TextChanged()=0;
    virtual void FontChanged()=0;
    virtual void ColorChanged()=0;
    virtual void OutColorChanged()=0;

protected:
    int         m_nFontSize=30;
    std::string m_sText;
    std::string m_sFont;
    SceneColor  m_stColor;
    SceneColor  m_stOutLineColor;
};

#endif// INTERFACE_HUD_TEXT_HEARDER_H
