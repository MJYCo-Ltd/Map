#ifndef INCLUDE_HEADER_LOADRESOURCE_H
#define INCLUDE_HEADER_LOADRESOURCE_H
#include <string>

namespace osg
{
    class Node;
    class Image;
    class Texture2D;
    class StateSet;
}

namespace osgText
{
    class Font;
}

class QImage;
struct RGBAData;

/// 状态集合类型
enum STATESET_TYPE
{
    BLEND_STATE=1,                /// 透明渲染
    LINE_STATE=1<<1,              /// 绘制线框模式
    FACE_STATE=1<<2,              /// 绘制面模式
    POLYGON_OFFSET_STATE=1<<3,    /// 多边形偏移
    GLOBAL_DRAW_STATE=1<<4,       /// 绘制图片着色器
    FLASH_DRAW_STATE=1<<5,        /// 闪烁着色器
    ATMOSPHERE_DRAW_STATE=1<<6,   /// 大气着色器
    POINT_DRAW_STATE=1<<7,        /// 点着色器
    LINE_DRAW_STATE=1<<8,         /// 线着色器
    PULSE_DRAW_STATE=1<<9,        /// 脉冲波着色器
    STAR_DRAW_STATE=1<<10,        /// 星星着色器
    SPACEENV_STATE=1<<11,         /// 太空着色器
    PHONGLIGHTING_STATE=1<<12,    /// 冯氏光照着色器
    VISUAL_STATE=1<<13,           /// 视域分析着色器
    FLAT_STATE=1<<14,             /// 平定模型着色器
    LIGHTING_STATE=1<<15,         /// 光照状态
    CULL_BACK=1<<16,              /// 开启背面裁剪
    DEPTH_TEST_OFF=1<<17,         /// 关闭深度测试
    DONT_WRIT_DEPTH=1<<18         /// 不写深度值
};

/**
 * @brief 资源加载器
 */
class ILoadResouce
{
public:
	virtual ~ILoadResouce(){}
    /**
     * @brief 初始化路径
     * @param csAppPath
     */
    virtual void InitPath(const std::string& csAppPath)=0;
    virtual const std::string& GetPath()=0;

    /**
     * @brief 加载模型
     * @param sModelPath 模型文件路径
     * @param bIsRef     是否是相对路径[相对GetPath()下的路径]
     * @return
     */
    virtual osg::Node* LoadNode(const std::string& sModelPath,bool bIsRef=true)=0;

    /**
     * @brief 加载纹理
     * @param sTexturePath 图片文件路径
     * @param bIsRef       是否是相对路径[相对GetPath()下的路径]
     * @return
     */
    virtual osg::Texture2D* LoadTexture(const std::string& sTexturePath, bool bIsRef=true)=0;
    virtual osg::Texture2D* LoadTexture(osg::Image*)=0;

    /**
     * @brief 加载字体
     * @param sFontPath 字体文件路径
     * @param bIsRef    是否是相对路径[相对GetPath()下的路径]
     * @return
     */
    virtual osgText::Font*  LoadFont(const std::string& sFontPath,bool bIsRef=true)=0;

    /**
     * @brief 加载图片
     * @param sImagePath  图片文件路径
     * @param nWidth      图片的宽度
     * @param nHeight     图片的高度
     * @param bIsRef      是否是相对路径[相对GetPath()下的路径]
     * @attention         如果图片的宽度和高度都是0则加载原始图片大小
     *                    否则将对图片进行缩放
     * @return
     */
    virtual osg::Image* LoadImage(const std::string& sImagePath, int nWidth=0, int nHeight=0,bool bIsRef=true)=0;
    virtual osg::Image* LoadImage(const QImage& rQImage)=0;
    virtual osg::Image* LoadImage(const RGBAData*)=0;

    /**
     * @brief 根据图片创建节点
     * @param sImagePath  图片文件路径
     * @param nWidth      图片的宽度
     * @param nHeight     图片的高度
     * @param bIsRef      是否是相对路径[相对GetPath()下的路径]
     * @attention         如果图片的宽度和高度都是0则加载原始图片大小
     *                    否则将对图片进行缩放
     * @return
     */
    virtual osg::Node* CreateImageNode(const std::string& sImagePath, int nWidth=0 , int nHeight=0, bool bIsRef=true)=0;
    virtual osg::Node* CreateImageNode(const QImage& rQImage)=0;
    virtual osg::Node* CreateImageNode(const RGBAData*)=0;

    /**
     * @brief 根据QImage产生某一值
     * @return
     */
    virtual std::string FindQImageKey(const QImage& rQImage)=0;

    /**
     * @brief 查找或者创建着色器
     * @return
     */
    virtual osg::StateSet* GetOrCreateStateSet(unsigned enType)=0;

    /**
     * @brief 清空不再使用的资源
     */
    virtual void ClearNoUse()=0;
};

#endif
