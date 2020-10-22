#ifndef INTERFACE_HUD_IMAGE_HEARDER_H
#define INTERFACE_HUD_IMAGE_HEARDER_H

#include <string>
#include <Hud/IHudNode.h>

/**
 * @brief 视口的屏显类
 */
class IHudImage:public IHudNode
{
public:
    CONSTRUCTOR(IHudImage,IHudNode)

    /**
     * @brief设置图片
     */
    void SetImage(const std::string& sText)JUDGE_EQUAL_CALL_FUNCTION(sText,m_sImagePath,ImageChanged)
    const std::string& Image()const{return(m_sImagePath);}
protected:
    virtual ~IHudImage(){}
    virtual void ImageChanged()=0;
protected:
    std::string m_sImagePath;
};

#endif/// INTERFACE_HUD_IMAGE_HEARDER_H
