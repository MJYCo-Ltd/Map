#include <QImage>
#include <osgDB/FileNameUtils>
#include <osgEarth/Shaders>
#include <osgEarth/VirtualProgram>
#include <VersionMathCommon.h>
#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osgEarth/GLUtils>
#include "ResourceLod.h"

osg::ref_ptr<osgEarth::VirtualProgram> g_pGlobal;

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
    else if(sFileName == "Atmosphere.glsl")
    {
        osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::get(pStateSete);
        if(nullptr != vp)
        {
            vp->setInheritShaders(false);
        }

        pStateSete->setAttributeAndModes(
                    new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL),
                    osg::StateAttribute::PROTECTED);

        osgEarth::GLUtils::setLighting(pStateSete, osg::StateAttribute::OFF);
        pStateSete->setAttributeAndModes( new osg::CullFace(osg::CullFace::FRONT), osg::StateAttribute::ON );
        pStateSete->setAttributeAndModes( new osg::Depth( osg::Depth::LESS, 0, 1, false ) ); // no depth write
        pStateSete->setAttributeAndModes( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false) ); // no zbuffer
        pStateSete->setAttributeAndModes( new osg::BlendFunc( GL_ONE, GL_ONE ), osg::StateAttribute::ON );

        float _innerRadius=osg::minimum(R_Earth2,R_Earth);
        // calculate and apply the uniforms:
        // TODO: perhaps we can just hard-code most of these as GLSL consts.
        float r_wl = ::powf( .65f, 4.0f );
        float g_wl = ::powf( .57f, 4.0f );
        float b_wl = ::powf( .475f, 4.0f );
        osg::Vec3 RGB_wl(1.0f/r_wl, 1.0f/g_wl, 1.0f/b_wl);
        float Kr = 0.0025f;
        float Kr4PI = Kr * 4.0f * osg::PI;
        float Km = 0.0015f;
        float Km4PI = Km * 4.0f * osg::PI;
        float ESun = 15.0f;
        float MPhase = -.095f;
        float RayleighScaleDepth = 0.25f;
        int   Samples = 2;
        float Weather = 1.0f;

        float Scale = 1.0f / (.025f * _innerRadius);

        pStateSete->getOrCreateUniform( "atmos_v3InvWavelength", osg::Uniform::FLOAT_VEC3 )->set(RGB_wl);
        pStateSete->getOrCreateUniform( "atmos_fInnerRadius",    osg::Uniform::FLOAT )->set(_innerRadius);
        pStateSete->getOrCreateUniform( "atmos_fInnerRadius2",   osg::Uniform::FLOAT )->set(_innerRadius * _innerRadius);
        pStateSete->getOrCreateUniform( "atmos_fKrESun",         osg::Uniform::FLOAT )->set(Kr * ESun);
        pStateSete->getOrCreateUniform( "atmos_fKmESun",         osg::Uniform::FLOAT )->set(Km * ESun);
        pStateSete->getOrCreateUniform( "atmos_fKr4PI",          osg::Uniform::FLOAT )->set(Kr4PI);
        pStateSete->getOrCreateUniform( "atmos_fKm4PI",          osg::Uniform::FLOAT )->set(Km4PI);
        pStateSete->getOrCreateUniform( "atmos_fScale",          osg::Uniform::FLOAT )->set(Scale);
        pStateSete->getOrCreateUniform( "atmos_fScaleDepth",     osg::Uniform::FLOAT )->set(RayleighScaleDepth);
        pStateSete->getOrCreateUniform( "atmos_fScaleOverScaleDepth", osg::Uniform::FLOAT )->set(Scale/RayleighScaleDepth);
        pStateSete->getOrCreateUniform( "atmos_g",               osg::Uniform::FLOAT )->set(MPhase);
        pStateSete->getOrCreateUniform( "atmos_g2",              osg::Uniform::FLOAT )->set(MPhase * MPhase);
        pStateSete->getOrCreateUniform( "atmos_nSamples",        osg::Uniform::INT )->set(Samples);
        pStateSete->getOrCreateUniform( "atmos_fSamples",        osg::Uniform::FLOAT )->set((float)Samples);
        pStateSete->getOrCreateUniform( "atmos_fWeather",        osg::Uniform::FLOAT )->set(Weather);
    }
}

/// 移除着色器
void CResourceLod::RemoveVirtualProgram(const std::string& sGLSLPath, osg::StateSet* pParentStateSet)
{
    if(nullptr == pParentStateSet)
    {
        return;
    }

    /// 如果该状态被多个人使用
    if(1 < pParentStateSet->getNumParents())
    {
    }

    std::string sFileName = osgDB::getSimpleFileName(sGLSLPath);
    static osgEarth::Util::Shaders shader;
    if(sFileName == "Flash.glsl")
    {
        pParentStateSet->removeUniform("flashStartTime");
        pParentStateSet->removeUniform("flashDurTime");
        pParentStateSet->removeUniform("flashIntervalTime");
        pParentStateSet->removeUniform("flashColor");
    }
    pParentStateSet->setAttribute(g_pGlobal.get());
}
