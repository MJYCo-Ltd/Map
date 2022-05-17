#ifndef INTERFACE_HUD_IMAGE_HEARDER_H
#define INTERFACE_HUD_IMAGE_HEARDER_H

#include <string>
#include <Hud/IHudNode.h>
#include <Plot/SceneShape/IImage.h>
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
    void SetRotation(double Angle)JUDGE_DOUBLE_CALL_FUNCTION(Angle,m_dAngle,AngleChanged);

    void SetImageSize(const SceneImageSize& stImageSize)JUDGE_EQUAL_CALL_FUNCTION(stImageSize,m_stImageSize,ImageSizeChanged);
protected:
    virtual ~IHudImage(){}
    virtual void ImageChanged()=0;
    virtual void AngleChanged()=0;
    virtual void ImageSizeChanged()=0;
protected:
    std::string m_sImagePath;
    double m_dAngle =0.0f;
    SceneImageSize m_stImageSize;
};

#endif/// INTERFACE_HUD_IMAGE_HEARDER_H
