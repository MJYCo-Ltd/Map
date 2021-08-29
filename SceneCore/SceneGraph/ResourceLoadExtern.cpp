#include <QImage>
#include <osgDB/FileNameUtils>
#include <osgEarth/Shaders>
#include <osgEarth/VirtualProgram>
#include "ResourceLod.h"

/// 将QImage转成OsgImage
osg::Image *CResourceLod::QImage2OsgImage(const QImage &rQImage)
{
    if(rQImage.format() != QImage::Format_RGBA8888)
    {
       QImage tmpImage = rQImage.convertToFormat(QImage::Format_RGBA8888);
       return(TransformQImage(tmpImage));
    }
    else
    {
        return(TransformQImage(rQImage));
    }
}

/// 转换QImage成osgImage
osg::Image *CResourceLod::TransformQImage(const QImage &rQImage)
{
    int nHeight = rQImage.height();
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    auto size = rQImage.byteCount();
#else
    auto size = rQImage.sizeInBytes();
#endif
    unsigned char* pTempBuffer = new unsigned char[size]();
    size = rQImage.bytesPerLine();
    for(int i=nHeight-1,j=0; i>-1; --i,++j)
    {
        memcpy(pTempBuffer+size*i,rQImage.scanLine(j),size);
    }

    auto image = new osg::Image;
    image->setImage(rQImage.width(), nHeight, 1, GL_RGBA,GL_RGBA, GL_UNSIGNED_BYTE,pTempBuffer,osg::Image::USE_NEW_DELETE);
    return(image);
}

/// 初始化状态信息
void CResourceLod::InitSateSet(osg::StateSet* pStateSete,const std::string& sFileName)
{
    if(sFileName == "Flash.glsl")
    {
        pStateSete->getOrCreateUniform("flashStartTime",osg::Uniform::FLOAT)->set((float)osg::Timer::instance()->time_s());
        pStateSete->getOrCreateUniform("flashDurTime",osg::Uniform::FLOAT)->set(.5f);
        pStateSete->getOrCreateUniform("flashIntervalTime",osg::Uniform::FLOAT)->set(1.f);
        pStateSete->getOrCreateUniform("flashColor",osg::Uniform::FLOAT_VEC4)->set(osg::Vec4(1.f,0.f,0.f,1.f));
    }
    else if(sFileName == "")
    {
    }
}

/// 移除着色器
void CResourceLod::RemoveVirtualProgram(const std::string& sGLSLPath, osg::StateSet* pParentStateSet, bool bIsRef)
{
    if(nullptr != pParentStateSet)
    {
        return;
    }

    auto pStateSet = LoadVirtualProgram(sGLSLPath,bIsRef);
    if(nullptr == pStateSet) return;

    std::string glslPath;

    if(bIsRef)
    {
        glslPath = m_sAppPath + sGLSLPath;
    }
    else
    {
        glslPath = sGLSLPath;
    }

    std::string sFileName = osgDB::getSimpleFileName(sGLSLPath);
    static osgEarth::Util::Shaders shader;
    if(sFileName == "Flash.glsl")
    {
        auto pVirtualProgram = dynamic_cast<osgEarth::VirtualProgram*>(pStateSet->getAttribute(osg::StateAttribute::PROGRAM));
        shader.unload(pVirtualProgram,glslPath);

        pParentStateSet->removeUniform("flashStartTime");
        pParentStateSet->removeUniform("flashDurTime");
        pParentStateSet->removeUniform("flashIntervalTime");
        pParentStateSet->removeUniform("flashColor");
    }
}
