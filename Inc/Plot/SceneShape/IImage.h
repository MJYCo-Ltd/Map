#ifndef INTERFACE_IMAGE_HEARDER_H
#define INTERFACE_IMAGE_HEARDER_H
#include <string>
#include <Plot/SceneShape/IShape.h>

class QImage;
/**
 * @brief 图片尺寸结构体
 */
struct SceneImageSize
{
    unsigned short unWidth=0u;  /// 图片宽度
    unsigned short unHeight=0u; /// 图片高度
    bool         bOutSet=false;///是否是外部设置

    bool operator == (const SceneImageSize& rOther)const
    {
        if(this == & rOther)
        {
            return (true);
        }

        return(unWidth == rOther.unWidth && unHeight == rOther.unHeight && bOutSet == rOther.bOutSet);
    }

    bool operator !=(const SceneImageSize& rOther)const
    {
        return(!this->operator==(rOther));
    }

    /// 写bool操作
    operator bool()const
    {
        return(0u !=unWidth && 0u !=unHeight);
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
    void SetImageSize(const SceneImageSize& stImageSize)JUDGE_EQUAL_CALL_FUNCTION(stImageSize,m_stImageSize,ImageSizeChanged)
    const SceneImageSize& ImageSize()const{return(m_stImageSize);}

    /**
     * @brief 设置图片的路径
     */
    void SetImagePath(const std::string& sImagePath)JUDGE_EQUAL_CALL_FUNCTION(sImagePath,m_sImagePath,ImagePathChanged)
    const std::string& ImagePath()const{return(m_sImagePath);}

    /**
     * @brief 设置内存图片
     */
    void SetRGBAData(const RGBAData& rRGBAData)JUDGE_EQUAL_CALL_FUNCTION(rRGBAData,m_stRGBAData,ImageDataChanged)

    /**
     * @brief 通过设置QImage设置图片内容
     * @param rImage
     */
    virtual void SetQImage(const QImage& rImage)=0;
protected:
    virtual ~IImage(){}

    /// 状态更改
    virtual void ImageSizeChanged()=0;
    virtual void ImagePathChanged()=0;
    virtual void ImageDataChanged()=0;

protected:
    std::string m_sImagePath;
    RGBAData    m_stRGBAData;
    SceneImageSize m_stImageSize;
};

#endif//INTERFACE_POINT_HEARDER_H
