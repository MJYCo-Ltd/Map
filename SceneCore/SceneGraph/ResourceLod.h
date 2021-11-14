#ifndef RESOURCELOD_H
#define RESOURCELOD_H

#include <QImage>
#include <osg/Texture2D>
#include <osg/Node>
#include <osgText/Font>

#include <Inner/ILoadResource.h>

class CResourceLod:public ILoadResouce
{
    friend class MyProxyNode;
public:
    CResourceLod();
    virtual ~CResourceLod(){}

    /**
     * @brief 初始化路径
     */
    void InitPath(const std::string& csAppPath);
    const std::string& GetPath(){return(m_sAppPath);}

    /**
     * @brief 加载模型
     */
    osg::Node* LoadNode(const std::string& sModelPath,bool bIsRef);

    /**
     * @brief 加载纹理
     */
    osg::Texture2D* LoadTexture(const std::string& sTexturePath, bool bIsRef);
    osg::Texture2D* LoadTexture(osg::Image*);

    /**
     * @brief 加载字体
     */
    osgText::Font*  LoadFont(const std::string& sFontPath,bool bIsRef);

    /**
     * @brief 加载图片
     */
    osg::Image* LoadImage(const std::string& sImagePath, int nWidth, int nHeight, bool bIsRef);
    osg::Image* LoadImage(const QImage& rQImage);
    osg::Image* LoadImage(const RGBAData* rImageData);

    /**
     * @brief 根据图片创建节点
     */
    osg::Node* CreateImageNode(const std::string& sImagePath, int nWidth, int nHeight, bool bIsRef);
    osg::Node* CreateImageNode(const QImage& rQImage);
    osg::Node* CreateImageNode(const RGBAData* rImageData);

    /**
     * @brief 根据QImage产生某一值
     * @return
     */
    virtual std::string FindQImageKey(const QImage& rQImage);

    osg::StateSet* GetOrCreateStateSet(unsigned enType);

    /**
     * @brief 清空不再使用的资源
     */
    void ClearNoUse();
protected:
    osg::Image* TransformQImage(const QImage& rQImage);
    osg::Node*  GetOrCreateNodeByImage(osg::Image* pImage);
    void InitSateSet(osg::StateSet* pStateSete,const std::string& sFileName);
    void LoadShader(const std::string& sGLSLPath, osg::StateSet *pStateSet);
protected:
    std::string m_sAppPath;
    std::map<std::string,osg::ref_ptr<osg::Node>>      m_mapNode;    /// 模型映射
    std::map<std::string,osg::ref_ptr<osg::Image>>     m_mapImage;   /// 图片映射
    std::map<qint64,osg::ref_ptr<osg::Image>>          m_mapQtImage; /// Qt图片映射
    std::map<std::string,osg::ref_ptr<osgText::Font>>  m_mapFont;    /// 字体映射
    std::map<osg::Image*,osg::ref_ptr<osg::Texture2D>> m_mapTexture; /// 纹理映射
    std::map<osg::Image*,osg::ref_ptr<osg::Node>>      m_mapImageNode;/// 图片节点
    std::map<unsigned,osg::ref_ptr<osg::StateSet>> m_mapType2StateSets;
};

#endif // RESOURCELOD_H
