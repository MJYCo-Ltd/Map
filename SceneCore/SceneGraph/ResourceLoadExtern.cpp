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
#include <osg/PointSprite>
#include <osg/PolygonOffset>
#include <osgViewer/View>
#include <SceneGraph/SceneType.h>
#include "ResourceLod.h"

class ViewPortChanged:public osg::StateSet::Callback
{
public:
    ViewPortChanged(osg::Uniform* pUniform):cameraSize(pUniform){}

    /// 回调函数
    void operator() (osg::StateSet*, osg::NodeVisitor*nv)
    {
        if(nv)
        {
            auto eventVisitor = nv->asEventVisitor();
            if(eventVisitor)
            {
                osgViewer::View* view = dynamic_cast<osgViewer::View*>(eventVisitor->getActionAdapter());
                osg::Camera* camera = (view!=0) ? view->getCamera() : 0;

                auto pViewPort = camera->getViewport();
                if(pViewPort)
                {
                    osg::Vec2 size(pViewPort->width(),pViewPort->height());
                    cameraSize->set(size);
                }
            }
        }
    }
private:
    osg::ref_ptr<osg::Uniform> cameraSize;
};

/// 根据类型创建渲染状态
osg::StateSet* CResourceLod::GetOrCreateStateSet(STATESET_TYPE enType)
{
    auto pFindone = m_mapType2StateSets.find(enType);
    if(m_mapType2StateSets.end() != pFindone)
    {
        return(pFindone->second);
    }
    else
    {
        osg::StateSet* pStateSet{nullptr};

        switch (enType)
        {
        case BLEND_STATE:
        {
            pStateSet= new osg::StateSet;
            pStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
            auto pDepth = new osg::Depth;
            pDepth->setWriteMask(false);
            pStateSet->setAttribute(pDepth);
            pStateSet->setAttributeAndModes(new osg::PolygonOffset(-1.f,-1.f));
            pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        }
            break;
        case FACE_STATE:
        {
            pStateSet= new osg::StateSet;
            pStateSet->setAttributeAndModes(
                        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL));
        }
            break;
        case LINE_STATE:
        {
            pStateSet= new osg::StateSet;
            pStateSet->setAttributeAndModes(
                        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));
        }
            break;

        case POLYGON_OFFSET_STATE:
        {
            pStateSet= new osg::StateSet;
            pStateSet->setAttributeAndModes(new osg::PolygonOffset(-1.f,-1.f));
        }
            break;
        }

        if(nullptr != pStateSet)
        {
            m_mapType2StateSets[enType] = pStateSet;
        }
        return(pStateSet);
    }
}

/// 将QImage转成OsgImage
osg::Image *CResourceLod::LoadImage(const QImage &rQImage)
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

/// 将根据内存中的数据构建图片
osg::Image *CResourceLod::LoadImage(const RGBAData *rImageData)
{
    auto pFindOne = m_mapImage.find(rImageData->sInnerName);
    if(m_mapImage.end() != pFindOne)
    {
        return(pFindOne->second);
    }
    else
    {
        int size = rImageData->unWidth*4;
        unsigned char* pTempBuffer = new unsigned char[size]();
        if(rImageData->bFlipVertically)
        {
            for(int i=rImageData->unHeight-1,j=0; i>j; --i,++j)
            {
                memcpy(pTempBuffer,rImageData->pRGBAData+j*size,size);
                memcpy(rImageData->pRGBAData+j*size,rImageData->pRGBAData+i*size,size);
                memcpy(rImageData->pRGBAData+i*size,pTempBuffer,size);
            }
        }
        delete[]pTempBuffer;

        osg::ref_ptr<osg::Image> pImage = new osg::Image;
        pImage->setImage(rImageData->unWidth, rImageData->unHeight, 1,
                         GL_RGBA,GL_RGBA, GL_UNSIGNED_BYTE,rImageData->pRGBAData,osg::Image::NO_DELETE);
        return(pImage);
    }
}
/// 将图片转成节点 begin
osg::Node* CResourceLod::CreateImageNode(const QImage& rQImage)
{
    return(GetOrCreateNodeByImage(LoadImage(rQImage)));
}

osg::Node *CResourceLod::CreateImageNode(const RGBAData *rImageData)
{
    return(GetOrCreateNodeByImage(LoadImage(rImageData)));
}

osg::Node *CResourceLod::CreateImageNode(const std::string &sImagePath,int nWidth,int nHeight,bool bIsRef)
{
    return(GetOrCreateNodeByImage(LoadImage(sImagePath,nWidth,nHeight,bIsRef)));
}
/// 将图片转成节点 end

/// 根据图片创建节点
osg::Node* CResourceLod::GetOrCreateNodeByImage(osg::Image* pImage)
{
    if(nullptr == pImage)
    {
        return(nullptr);
    }

    auto pFind = m_mapImageNode.find(pImage);
    if(m_mapImageNode.end()!= pFind)
    {
        return(pFind->second);
    }
    else
    {
        auto pVertexColor = new osg::Vec4Array(1);
        auto pVertexArray = new osg::Vec3Array(4);
        auto pTexCoordArray = new osg::Vec2Array(4);

        auto pDrawArray = new osg::DrawArrays(GL_TRIANGLE_STRIP,0,4);
        auto pGeometry = new osg::Geometry;

        pVertexColor->at(0).set(1.f,1.f,1.f,1.f);

        pGeometry->addPrimitiveSet(pDrawArray);
        pGeometry->setTexCoordArray(0,pTexCoordArray);
        pGeometry->setVertexArray(pVertexArray);
        pGeometry->setColorArray(pVertexColor,osg::Array::BIND_OVERALL);

        pTexCoordArray->at(1).set(0,1);
        pTexCoordArray->at(2).set(1,0);
        pTexCoordArray->at(3).set(1,1);

        auto pTexture = LoadTexture(pImage);

        int nX(pImage->s()/2);
        int nY(pImage->t()/2);

        pVertexArray->at(0).set(-nX,-nY,0);
        pVertexArray->at(1).set(-nX,nY,0);
        pVertexArray->at(2).set(nX,-nY,0);
        pVertexArray->at(3).set(nX,nY,0);

        pGeometry->getOrCreateStateSet()->setTextureAttribute(0,pTexture);
        pGeometry->getOrCreateStateSet()->setMode(GL_BLEND,true);
        auto pDepth = new osg::Depth;
        pDepth->setWriteMask(false);
        pGeometry->getOrCreateStateSet()->setAttribute(pDepth);
        pGeometry->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonOffset(-1.f,-1.f));
        pGeometry->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        m_mapImageNode[pImage] = pGeometry;
        return(pGeometry);
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
        pStateSete->setAttribute( new osg::Depth(osg::Depth::ALWAYS, 0, 1, false) ); // no zbuffer
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
    else if(sFileName == "Point.glsl")
    {
        pStateSete->getOrCreateUniform("pointSize",osg::Uniform::FLOAT)->set(1.0f);
        pStateSete->setMode(GL_VERTEX_PROGRAM_POINT_SIZE,1);
        auto pSprite = new osg::PointSprite();
        pSprite->setCoordOriginMode(osg::PointSprite::LOWER_LEFT);
        pStateSete->setTextureAttributeAndModes(0, pSprite, osg::StateAttribute::ON);
    }
    else if(sFileName == "Line.glsl")
    {
        osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::get(pStateSete);
        if(nullptr != vp)
        {
            vp->addBindAttribLocation("Line_prev_point",osg::Drawable::ATTRIBUTE_6);
            vp->addBindAttribLocation("Line_next_point",osg::Drawable::ATTRIBUTE_7);
        }

        pStateSete->getOrCreateUniform("LineWidth",osg::Uniform::FLOAT)->set(1.0f);
        pStateSete->getOrCreateUniform("LineStipplePattern",osg::Uniform::INT)->set(0xFFFF);
        pStateSete->getOrCreateUniform("LineStippleFactor",osg::Uniform::INT)->set(1);
        pStateSete->getOrCreateUniform("cameraSize",osg::Uniform::FLOAT_VEC2);
        pStateSete->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);
        pStateSete->setEventCallback(new ViewPortChanged(pStateSete->getUniform("cameraSize")));
    }
    else if(sFileName == "Pulse.glsl")
    {
        pStateSete->getOrCreateUniform("bIsOut",osg::Uniform::BOOL)->set(true);
        pStateSete->getOrCreateUniform("fPulseStartTime",osg::Uniform::FLOAT)->set((float)osg::Timer::instance()->time_s());
        pStateSete->getOrCreateUniform("fPulseIntervalTime",osg::Uniform::FLOAT)->set(1.f);
        pStateSete->getOrCreateUniform("fPulseStep",osg::Uniform::FLOAT)->set(.1f);
    }

    osgEarth::GLUtils::setLighting(pStateSete,osg::StateAttribute::OFF);
}
