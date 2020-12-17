#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osg/Group>
#include <osgEarth/Shaders>
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
bool CResourceLod::LoadVirtualProgram(osgEarth::VirtualProgram* pVirtualProgram,const std::string& sGLSLPath,bool bIsRef)
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


    static osgEarth::Shaders shader;
    return(shader.load(pVirtualProgram,glslPath));
}

/// 移除virtualProgram
bool CResourceLod::RemoveVirtualProgram(osgEarth::VirtualProgram *pVirtualProgram, const std::string &sGLSLPath, bool bIsRef)
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


    static osgEarth::Shaders shader;
    return(shader.unload(pVirtualProgram,glslPath));
}
