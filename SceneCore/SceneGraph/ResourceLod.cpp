#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osg/Group>
#include <osgUtil/MeshOptimizers>
#include <osg/ProxyNode>
#include <osgEarth/Shaders>
#include <osgEarth/MapNode>
#include <osgEarth/Lighting>
#include <osgEarth/Utils>
#include <osgEarth/VirtualProgram>

#include "ResourceLod.h"

class MyProxyNode:public osg::ProxyNode
{
public:
    MyProxyNode(CResourceLod* pResourceLoad,const std::string& sModelPath):
        m_pResourceLoad(pResourceLoad),m_sModelPath(sModelPath)
    {
        setFileName(0,sModelPath);
        setCenterMode(UNION_OF_BOUNDING_SPHERE_AND_USER_DEFINED);
        setRadius(100.f);
    }

    virtual bool addChild(Node *child)
    {
        if(osg::ProxyNode::addChild(child))
        {
            auto pStateSet = m_pResourceLoad->LoadVirtualProgram("GLSL/Global.glsl");

            m_pResourceLoad->m_mapNode[m_sModelPath] = child;
            osgEarth::GenerateGL3LightingUniforms generateUniforms;
            child->accept(generateUniforms);

            if(child->getStateSet())
            {
                child->setStateSet(m_pResourceLoad->MergeStateSet(pStateSet,child->getStateSet()));
            }
            else
            {
                child->setStateSet(pStateSet);
            }

            return(true);
        }

        return(false);
    }

protected :
    virtual ~MyProxyNode() {}
private:
    CResourceLod* m_pResourceLoad;
    std::string   m_sModelPath;
};

static std::set<std::string> s_gNeedAss;
static const std::string s_gEarth("earth");

/// 初始化路径
CResourceLod::CResourceLod()
{
    s_gNeedAss.insert("fbx");
    s_gNeedAss.insert("obj");
}

void CResourceLod::InitPath(const std::string &csAppPath)
{
    m_sAppPath = csAppPath;
}

/// 加载模型
osg::Node *CResourceLod::LoadNode(const std::string &sModelPath,bool bIsRef)
{
    std::string modelPath;
    if(bIsRef)
    {
        modelPath = m_sAppPath + sModelPath;
    }
    else
    {
        modelPath = sModelPath;
    }

    auto fileExten = osgDB::getLowerCaseFileExtension(modelPath);
    auto pFineOne = s_gNeedAss.find(fileExten);
    if(s_gNeedAss.end() != pFineOne)
    {
        modelPath +=".ass";
    }


    auto itor = m_mapNode.find(modelPath);
    if(m_mapNode.end() != itor && itor->second.valid())
    {
        return(itor->second.get());
    }
    else
    {
        if(s_gEarth == fileExten)
        {
            osg::Node* pNode = osgDB::readNodeFile(modelPath);
            if(nullptr != pNode)
            {
                m_mapNode[modelPath] = pNode;
            }

            return(pNode);
        }
        else
        {
            MyProxyNode* pNode = new MyProxyNode(this,modelPath);//osgDB::readNodeFile(modelPath);
            m_mapNode[modelPath] = pNode;
            return(pNode);
        }
    }
}

/// 加载纹理
osg::Texture2D *CResourceLod::LoadTexture(const std::string &sTexturePath,bool bIsRef)
{
    std::string texturePath;
    if(bIsRef)
    {
        texturePath = m_sAppPath + sTexturePath;
    }
    else
    {
        texturePath = sTexturePath;
    }

    auto itor = m_mapTexture.find(texturePath);
    if(m_mapTexture.end() != itor)
    {
        return(itor->second.get());
    }
    else
    {
        osg::Image* pImage = LoadImage(texturePath,0,0,false);
        osg::Texture2D* pTexture = nullptr;
        if(nullptr != pImage)
        {
            pTexture = new osg::Texture2D;
            pTexture->setImage(pImage);
            pTexture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
            pTexture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
            pTexture->setResizeNonPowerOfTwoHint(false);

            m_mapTexture[texturePath] = pTexture;
        }

        return(pTexture);
    }
}

/// 加载字体
osgText::Font *CResourceLod::LoadFont(const std::string &sFontPath, bool bIsRef)
{
    std::string fontPath;
    if(bIsRef)
    {
        fontPath = m_sAppPath + sFontPath;
    }
    else
    {
        fontPath = sFontPath;
    }

    auto itor = m_mapFont.find(fontPath);
    if(m_mapFont.end() != itor)
    {
        return(itor->second.get());
    }
    else
    {
        osgText::Font* pFont = osgText::readFontFile(fontPath);
        if(nullptr != pFont)
        {
            m_mapFont[fontPath] = pFont;
        }

        return(pFont);
    }
}

/// 读取图片
osg::Image *CResourceLod::LoadImage(const std::string &sImagePath, int nWidth, int nHeight, bool bIsRef)
{
    static std::string imagePath;
    static bool bScale;

    if(bIsRef)
    {
        imagePath = m_sAppPath + sImagePath;
    }
    else
    {
        imagePath = sImagePath;
    }

    std::string sForFind = imagePath;
    std::map<std::string,osg::ref_ptr<osg::Image> >::iterator itor;
    if(0 != nWidth && 0 != nHeight)
    {
        bScale = true;
    }
    else
    {
        bScale = false;
    }

    if(bScale)
    {
        char buffer[255]="";
        sprintf(buffer,"%d",nWidth);
        sForFind += "_";
        sForFind += buffer;
        sForFind += "_";
        sprintf(buffer,"%d",nHeight);
        sForFind += buffer;
    }

    itor = m_mapImage.find(sForFind);

    if(m_mapImage.end() != itor && itor->second.valid())
    {
        return(itor->second.get());
    }
    else
    {
        osg::ref_ptr<osg::Image> pImage = osgDB::readImageFile(imagePath);

        if(nullptr != pImage)
        {
            if(bScale)
            {
                pImage->scaleImage(nWidth, nHeight, pImage->r());
            }

            m_mapImage[sForFind] = pImage;
        }

        return(pImage);
    }
}

/// 加载virtualProgram
osg::StateSet* CResourceLod::LoadVirtualProgram(const std::string& sGLSLPath,bool bIsRef)
{
    std::string glslPath;

    if(bIsRef)
    {
        glslPath = m_sAppPath + sGLSLPath;
    }
    else
    {
        glslPath = sGLSLPath;
    }

    auto itor = m_mapStateSet.find(glslPath);

    if(m_mapStateSet.end() != itor && itor->second.valid())
    {
        return(itor->second.get());
    }
    else
    {
        osg::StateSet* pParentStateSet = new osg::StateSet;
        static osgEarth::Util::Shaders shader;

        /// 创建方程
        auto pVirtualProgram = osgEarth::VirtualProgram::getOrCreate(pParentStateSet);
        shader.load(pVirtualProgram,glslPath);

        InitSateSet(pParentStateSet,osgDB::getSimpleFileName(glslPath));

        m_mapStateSet[glslPath] = pParentStateSet;

        return(pParentStateSet);
    }
}

/// 合并状态
osg::StateSet *CResourceLod::MergeStateSet(osg::StateSet *pParent, osg::StateSet *pStateSet)
{
    /// 构造混合了两个状态的状态集合
    osg::ref_ptr<osg::StateSet> pChildStateSet = new osg::StateSet;
    pChildStateSet->merge(*pParent);
    pChildStateSet->merge(*pStateSet);

    /// 如果合并状态跟没合并之前一致，则直接返回
    if(0 == pChildStateSet->compare(*pParent,true))
    {
        return(pParent);
    }

    auto findOne = m_mapMergeStateSet.find(pParent);
    if(m_mapMergeStateSet.end() != findOne)
    {
        /// 遍历所有的状态集合，如果有状态相等的则直接返回
        for(auto one=findOne->second.begin();one!=findOne->second.end();++one)
        {
            if(0 == pChildStateSet->compare(**one,true))
            {
                return(*one);
            }
        }

        /// 如果没有找到则新建
        findOne->second.push_back(pChildStateSet);

        return(pChildStateSet);
    }
    else
    {
        m_mapMergeStateSet[pParent].push_back(pChildStateSet);
        return(pChildStateSet);
    }
}

template <typename T>
void ClearMap(T& value)
{
    for(auto one = value.begin();one != value.end();)
    {
        if(1 == one->second->referenceCount())
        {
            one = value.erase(one);
        }
        else
        {
            ++one;
        }
    }
}

/// 清空不再使用资源
void CResourceLod::ClearNoUse()
{
    ClearMap(m_mapNode);
    ClearMap(m_mapImage);
    ClearMap(m_mapTexture);
    ClearMap(m_mapFont);
    ClearMap(m_mapStateSet);
}
