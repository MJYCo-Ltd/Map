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
     * @brief 加载着色器
     * @param sGLSLPath  着色器文件路径
     * @param bIsRef     是否是相对路径 [相对GetPath()下的路径]
     * @return
     */
    virtual osg::StateSet* GetOrCreateStateSet(const std::string& sGLSLPath,bool bIsRef=true)=0;

    /**
     * @brief 清空不再使用的资源
     */
    virtual void ClearNoUse()=0;
};

#endif
