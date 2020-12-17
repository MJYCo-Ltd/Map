#ifndef INCLUDE_HEADER_LOADRESOURCE_H
#define INCLUDE_HEADER_LOADRESOURCE_H
#include <string>

namespace osg
{
    class Node;
    class Image;
    class Texture2D;
}

namespace osgEarth
{
    class VirtualProgram;
}

namespace osgText
{
    class Font;
}

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
     * @brief 加载着色器
     * @param sGLSLPath
     * @param bIsRef
     * @return
     */
    virtual bool LoadVirtualProgram(osgEarth::VirtualProgram* pVirtualProgram,const std::string& sGLSLPath,bool bIsRef=true)=0;
    virtual bool RemoveVirtualProgram(osgEarth::VirtualProgram* pVirtualProgram,const std::string& sGLSLPath,bool bIsRef=true)=0;
};

#endif
