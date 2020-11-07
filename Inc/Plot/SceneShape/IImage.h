#ifndef INTERFACE_IMAGE_HEARDER_H
#define INTERFACE_IMAGE_HEARDER_H
#include <string>
#include <Plot/SceneShape/IShape.h>

struct SceneImageSize
{
    unsigned int unWidth=0u;  /// 图片宽度
    unsigned int unHeight=0u; /// 图片高度

    bool operator == (const SceneImageSize& rOther)const
    {
        if(this == & rOther)
        {
            return (true);
        }

        return(unWidth == rOther.unWidth && unHeight == rOther.unHeight);
    }

    bool operator !=(const SceneImageSize& rOther)const
    {
        return(!this->operator==(rOther));
    }

    /// 写bool操作
    operator bool()const
    {
        return(0!=unWidth && 0!=unHeight);
    }
};

/**
 * @brief 场景图片类
 */
class IImage:public IShape
{
public:
    CONSTRUCTOR(IImage,IShape)
    /**
     * @brief 设置图片的大小
     */
    void SetImageSize(const SceneImageSize& stImageSize)JUDGE_EQUAL_CALL_FUNCTION(stImageSize,m_stImageSize,ImagePathChanged)
    const SceneImageSize& ImageSize()const{return(m_stImageSize);}

    /**
     * @brief 设置图片的路径
     */
    void SetImagePath(const std::string& sImagePath)JUDGE_EQUAL_CALL_FUNCTION(sImagePath,m_sImagePath,ImagePathChanged)
    const std::string& ImagePath()const{return(m_sImagePath);}
protected:
    virtual ~IImage(){}

    /// 状态更改
    virtual void ImageSizeChanged()=0;
    virtual void ImagePathChanged()=0;

protected:
    std::string m_sImagePath;
    SceneImageSize m_stImageSize;
};

#endif//INTERFACE_POINT_HEARDER_H
