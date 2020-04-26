#ifndef INCLUDE_HEADER_LOADRESOURCE_H
#define INCLUDE_HEADER_LOADRESOURCE_H
#include <string>
using namespace std;

namespace osg
{
    class Node;
    class Image;
    class Texture2D;
}

namespace osgText
{
    class Font;
}

class ILoadResouce
{
public:
    virtual ~ILoadResouce(){}

    /**
     * @brief 初始化路径
     * @param csAppPath
     */
    virtual void InitPath(const string& csAppPath)=0;
    virtual const string& GetPath()=0;

    /**
     * @brief 加载模型
     * @param sModelPath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    virtual osg::Node* LoadNode(const string& sModelPath,bool bIsRef=true)=0;

    /**
     * @brief 加载纹理
     * @param sTexturePath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    virtual osg::Texture2D* LoadTexture(const string& sTexturePath, bool bIsRef=true)=0;

    /**
     * @brief 加载字体
     * @param sFontPath 相对于exe所在的路径
     * @param bIsRef    是否是相对路径
     * @return
     */
    virtual osgText::Font*  LoadFont(const string& sFontPath,bool bIsRef=true)=0;

    /**
     * @brief 加载图片
     * @param sImagePath
     * @param bIsRef
     * @return
     */
    virtual osg::Image* LoadImage(const string& sImagePath, int nWidth=0, int nHeight=0,bool bIsRef=true)=0;
};

#endif
