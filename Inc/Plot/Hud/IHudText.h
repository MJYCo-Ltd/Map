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
	
	void SetText(const std::string& sText);
	const std::string& Text()const{return(m_sText);}
	
	void SetFont(const std::string& sFont);
	const std::string& Font()const{return(m_sFont);}
	
	void SetColor(const SceneColor& rColor);
	const SceneColor& Color()const{return(m_stColor);}
	
	void SetOutColor(const SceneColor& rColor);
	const SceneColor& OutLineColor()const{return(m_stOutLineColor);}
	
	void SetFontSize(int nFontSize);
	int  FontSize() const{return(m_nFontSize);}
protected:
    virtual ~IHudText(){}
	
protected:
    int         m_nFontSize;
	std::string m_sText;
	std::string m_sFont;
	SceneColor  m_stColor;
	SceneColor  m_stOutLineColor;
};

#endif// INTERFACE_HUD_TEXT_HEARDER_H
