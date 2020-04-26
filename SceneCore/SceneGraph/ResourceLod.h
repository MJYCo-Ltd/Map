#ifndef RESOURCELOD_H
#define RESOURCELOD_H

#include <osg/Texture2D>
#include <osg/Node>
#include <osgText/Font>

#include <Inner/ILoadResource.h>
class CResourceLod:public ILoadResouce
{
public:
    CResourceLod();
    virtual ~CResourceLod();

    /**
     * @brief 初始化路径
     * @param csAppPath
     */
    void InitPath(const string& csAppPath);
    const std::string& GetPath(){return(m_sAppPath);}

    /**
     * @brief 加载模型
     * @param sModelPath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    osg::Node* LoadNode(const string& sModelPath,bool bIsRef);

    /**
     * @brief 加载纹理
     * @param sTexturePath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    osg::Texture2D* LoadTexture(const string& sTexturePath, bool bIsRef);

    /**
     * @brief 加载字体
     * @param sFontPath 相对于exe所在的路径
     * @param bIsRef    是否是相对路径
     * @return
     */
    osgText::Font*  LoadFont(const string& sFontPath,bool bIsRef);

    /**
     * @brief 加载图片
     * @param sImagePath
     * @param bIsRef
     * @return
     */
    osg::Image* LoadImage(const string& sImagePath, int nWidth, int nHeight, bool bIsRef);

protected:
    string m_sAppPath;
    std::map<string,osg::observer_ptr<osg::Node> >      m_mapNode;    /// 模型映射
    std::map<string,osg::ref_ptr<osg::Image> >     m_mapImage;   /// 图片映射
    std::map<string,osg::observer_ptr<osg::Texture2D> > m_mapTexture; /// 纹理映射
    std::map<string,osg::observer_ptr<osgText::Font> >  m_mapFont;    /// 字体映射
};

#endif // RESOURCELOD_H
