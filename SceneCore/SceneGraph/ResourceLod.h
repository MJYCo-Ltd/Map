#ifndef RESOURCELOD_H
#define RESOURCELOD_H

#include <osg/Texture2D>
#include <osg/Node>
#include <osgText/Font>

#include <Inner/ILoadResource.h>
class QImage;

class CResourceLod:public ILoadResouce
{
    friend class MyProxyNode;
public:
    CResourceLod();
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
    osg::StateSet* LoadVirtualProgram(const std::string& sGLSLPath,bool bIsRef=true);

    /**
     * @brief 合并渲染状态
     * @param pParent 原始状态
     * @param pChild  子状态
     * @return
     */
    osg::StateSet* MergeStateSet(osg::StateSet* pParent,osg::StateSet* pStateSet);

    /**
     * @brief 移除着色器
     * @param sGLSLPath
     * @param pParentStateSet
     */
    void RemoveVirtualProgram(const std::string& sGLSLPath, osg::StateSet* pParentStateSet);

    /**
     * @brief 清空不再使用的资源
     */
    void ClearNoUse();
protected:
    osg::Image* TransformQImage(const QImage& rQImage);
    void InitSateSet(osg::StateSet* pStateSete,const std::string& sFileName);
protected:
    std::string m_sAppPath;
    std::map<std::string,osg::ref_ptr<osg::Node>>      m_mapNode;    /// 模型映射
    std::map<std::string,osg::ref_ptr<osg::Image>>     m_mapImage;   /// 图片映射
    std::map<std::string,osg::ref_ptr<osg::Texture2D>> m_mapTexture; /// 纹理映射
    std::map<std::string,osg::ref_ptr<osgText::Font>>  m_mapFont;    /// 字体映射
    std::map<std::string,osg::ref_ptr<osg::StateSet>>  m_mapStateSet; /// shader程序加载的状态集合
    std::map<osg::observer_ptr<osg::StateSet>,
    std::list<osg::ref_ptr<osg::StateSet>>>           m_mapMergeStateSet;
};

#endif // RESOURCELOD_H
