#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osg/Group>
#include "ResourceLod.h"

/// 初始化路径
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

    auto itor = m_mapNode.find(modelPath);
    if(m_mapNode.end() != itor && itor->second.valid())
    {
        return(itor->second.get());
    }
    else
    {
        osg::Node* pNode = osgDB::readNodeFile(modelPath);

        if(nullptr != pNode)
        {
            m_mapNode[modelPath] = pNode;
        }

        return(pNode);
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
    std::string imagePath;
    if(bIsRef)
    {
        imagePath = m_sAppPath + sImagePath;
    }
    else
    {
        imagePath = sImagePath;
    }

    string sForFind = imagePath;
    std::map<string,osg::ref_ptr<osg::Image> >::iterator itor;
    if(0 != nWidth && 0 != nHeight)
    {
        char buffer[255]="";
        itoa(nWidth,buffer,10);
        sForFind += "_";
        sForFind += buffer;
        sForFind += "_";
        itoa(nHeight,buffer,10);
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
            pImage->scaleImage(nWidth, nHeight, pImage->r());
            m_mapImage[sForFind] = pImage;
        }

        return(pImage);
    }
}

CResourceLod::CResourceLod()
{
}

CResourceLod::~CResourceLod()
{
}
