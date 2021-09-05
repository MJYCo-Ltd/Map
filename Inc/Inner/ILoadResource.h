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
     * @param sModelPath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    virtual osg::Node* LoadNode(const std::string& sModelPath,bool bIsRef=true)=0;

    /**
     * @brief 加载纹理
     * @param sTexturePath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    virtual osg::Texture2D* LoadTexture(const std::string& sTexturePath, bool bIsRef=true)=0;

    /**
     * @brief 加载字体
     * @param sFontPath 相对于exe所在的路径
     * @param bIsRef    是否是相对路径
     * @return
     */
    virtual osgText::Font*  LoadFont(const std::string& sFontPath,bool bIsRef=true)=0;

    /**
     * @brief 加载图片
     * @param sImagePath
     * @param bIsRef
     * @return
     */
    virtual osg::Image* LoadImage(const std::string& sImagePath, int nWidth=0, int nHeight=0,bool bIsRef=true)=0;

    /**
     * @brief 将QImage转换成OsgImage
     * @return osg::Image指针,外部负责释放
     */
    virtual osg::Image* QImage2OsgImage(const QImage& rQImage)=0;

    /**
     * @brief 加载着色器
     * @param sGLSLPath
     * @param bIsRef
     * @return
     */
    virtual osg::StateSet* GetOrCreateStateSet(const std::string& sGLSLPath,bool bIsRef=true)=0;

    /**
     * @brief 清空不再使用的资源
     */
    virtual void ClearNoUse()=0;
};

#endif
