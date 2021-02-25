#ifndef RESOURCELOD_H
#define RESOURCELOD_H

#include <QImage>
#include <osg/Texture2D>
#include <osg/Node>
#include <osgText/Font>
#include <osgEarth/VirtualProgram>

#include <Inner/ILoadResource.h>
class CResourceLod:public ILoadResouce
{
public:
    CResourceLod(){}
    virtual ~CResourceLod(){}

    /**
     * @brief 初始化路径
     * @param csAppPath
     */
    void InitPath(const std::string& csAppPath);
    const std::string& GetPath(){return(m_sAppPath);}

    /**
     * @brief 加载模型
     * @param sModelPath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    osg::Node* LoadNode(const std::string& sModelPath,bool bIsRef);

    /**
     * @brief 加载纹理
     * @param sTexturePath 相对于exe所在的路径
     * @param bIsRef       是否是相对路径
     * @return
     */
    osg::Texture2D* LoadTexture(const std::string& sTexturePath, bool bIsRef);

    /**
     * @brief 加载字体
     * @param sFontPath 相对于exe所在的路径
     * @param bIsRef    是否是相对路径
     * @return
     */
    osgText::Font*  LoadFont(const std::string& sFontPath,bool bIsRef);

    /**
     * @brief 加载图片
     * @param sImagePath
     * @param bIsRef
     * @return
     */
    osg::Image* LoadImage(const std::string& sImagePath, int nWidth, int nHeight, bool bIsRef);
    osg::Image* QImage2OsgImage(const QImage& rQImage);

    /**
     * @brief 加载着色器
     * @param sGLSLPath
     * @param bIsRef
     * @return
     */
    bool LoadVirtualProgram(osgEarth::VirtualProgram* pVirtualProgram,const std::string& sGLSLPath,bool bIsRef=true);
    bool RemoveVirtualProgram(osgEarth::VirtualProgram* pVirtualProgram,const std::string& sGLSLPath,bool bIsRef=true);
protected:
    osg::Image* TransformQImage(const QImage& rQImage);
protected:
    std::string m_sAppPath;
    std::map<std::string,osg::observer_ptr<osg::Node>>      m_mapNode;    /// 模型映射
    std::map<std::string,osg::ref_ptr<osg::Image>>     m_mapImage;   /// 图片映射
    std::map<std::string,osg::observer_ptr<osg::Texture2D>> m_mapTexture; /// 纹理映射
    std::map<std::string,osg::observer_ptr<osgText::Font>>  m_mapFont;    /// 字体映射
    std::map<std::string,osg::observer_ptr<osgEarth::VirtualProgram>>m_mapProgram; /// 纹理
};

#endif // RESOURCELOD_H
