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
osg::StateSet* CResourceLod::GetOrCreateStateSet(unsigned unType)
{
    if(0 == unType)
    {
        return(nullptr);
    }

    auto pFindone = m_mapType2StateSets.find(unType);
    if(m_mapType2StateSets.end() != pFindone)
    {
        return(pFindone->second);
    }
    else
    {
        osg::StateSet* pStateSet=new osg::StateSet;

        /// 开启光照
        if(unType & LIGHTING_STATE)
        {
            osgEarth::GLUtils::setLighting(pStateSet, osg::StateAttribute::ON);
        }
        else
        {
            osgEarth::GLUtils::setLighting(pStateSet, osg::StateAttribute::OFF);
        }

        /// 如果透明开启
        if(unType & BLEND_STATE)
        {
            pStateSet->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
            pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        }

        /// 如果开启多边形偏移
        if(unType & POLYGON_OFFSET_STATE)
        {
            pStateSet->setAttributeAndModes(new osg::PolygonOffset(-1.f,-1.f));
        }

        /// 开启画面模式
        if(unType & FACE_STATE)
        {
            pStateSet->setAttributeAndModes(
                        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL));
        }

        /// 开启画线模式
        if(unType &LINE_STATE)
        {
            pStateSet->setAttributeAndModes(
                        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE));
        }

        if(unType & CULL_BACK)
        {
            pStateSet->setAttributeAndModes(new osg::CullFace);
        }

        if(unType & DONT_WRIT_DEPTH)
        {
            auto pDepth = new osg::Depth;
            pDepth->setWriteMask(false);
            pStateSet->setAttribute(pDepth);
        }

        /// 添加全局着色器
        if(unType & GLOBAL_DRAW_STATE)
        {
            LoadShader("GLSL/Global.glsl",pStateSet);
        }

        /// 添加闪烁着色器
        if(unType & FLASH_DRAW_STATE)
        {
            LoadShader("GLSL/Flash.glsl",pStateSet);
        }

        /// 添加大气着色器
        if(unType & ATMOSPHERE_DRAW_STATE)
        {
            LoadShader("GLSL/Atmosphere.glsl",pStateSet);
        }

        /// 添加点着色器
        if(unType & POINT_DRAW_STATE)
        {
            LoadShader("GLSL/Point.glsl",pStateSet);
        }

        /// 添加线着色器
        if(unType & LINE_DRAW_STATE)
        {
            LoadShader("GLSL/Line.glsl",pStateSet);
        }

        /// 添加脉冲波着色器
        if(unType & PULSE_DRAW_STATE)
        {
            LoadShader("GLSL/Pulse.glsl",pStateSet);
        }

        /// 添加恒星着色器
        if(unType & STAR_DRAW_STATE)
        {
            LoadShader("GLSL/Star.glsl",pStateSet);
        }

        /// 添加太空着色器
        if(unType & SPACEENV_STATE)
        {
            LoadShader("GLSL/SpaceEnv.glsl",pStateSet);
        }

        /// 添加冯氏光照着色器
        if(unType & PHONGLIGHTING_STATE)
        {
            LoadShader("GLSL/PhongLighting.glsl",pStateSet);
        }

        /// 添加视域分析着色器
        if(unType & VISUAL_STATE)
        {
            LoadShader("GLSL/Visual.glsl",pStateSet);
        }

        /// 添加平定模型着色器
        if(unType & FLAT_STATE)
        {
            LoadShader("GLSL/Flat.glsl",pStateSet);
        }

        if(unType &DEPTH_TEST_OFF)
        {
            pStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
        }
        m_mapType2StateSets[unType] = pStateSet;
        return(pStateSet);
    }
}

/// 计算QImage的统一变量
uint CResourceLod::UniqueImage(const QImage& rQImage)
{
    uint uHash(0);

    /// 此方法copy自QImage的判等运算符
    QImage::Format emFormat = rQImage.format();
    if (emFormat != QImage::Format_RGB32)
    {
        if(emFormat >= QImage::Format_ARGB32)
        {
            const int n = rQImage.width() * rQImage.depth() / 8;
            const uchar *bits = rQImage.constBits();
            if (n == rQImage.bytesPerLine())
            {
                for(int i=rQImage.sizeInBytes()-1;i>-1;--i)
                {
                    uHash += bits[i];
                    uHash += (uHash << 10);
                    uHash ^= (uHash >> 6);
                }
            }
            else
            {
                for (int y = 0; y < rQImage.height(); ++y)
                {
                    bits = rQImage.scanLine(y);
                    for(int i=n-1;i>-1;--i)
                    {
                        uHash += bits[i];
                        uHash += (uHash << 10);
                        uHash ^= (uHash >> 6);
                    }
                }
            }
        }
        else
        {
            const int w = rQImage.width();
            const int h = rQImage.height();
            QVector<QRgb> colortable = rQImage.colorTable();
            for (int y=0; y<h; ++y)
            {
                for (int x=0; x<w; ++x)
                {
                    uHash += colortable[rQImage.pixelIndex(x, y)];
                    uHash += (uHash << 10);
                    uHash ^= (uHash >> 6);
                }
            }
        }
    }
    else
    {
        for(int l = rQImage.height()-1; l >-1 ; --l)
        {
            int w = rQImage.width();
            const uint *p1 = reinterpret_cast<const uint*>(rQImage.scanLine(l));
            while (w--)
            {
                uHash += *p1++;
                uHash += (uHash << 10);
                uHash ^= (uHash >> 6);
            }
        }
    }

    return(uHash);
}

/// 将QImage转成OsgImage
osg::Image *CResourceLod::LoadImage(const QImage &rQImage)
{
    uint uHashKey = UniqueImage(rQImage);
    auto pFindOne = m_mapQtImage.find(uHashKey);
    if(m_mapQtImage.end() != pFindOne)
    {
        return(pFindOne->second);
    }
    else
    {
        if(rQImage.format() != QImage::Format_RGBA8888)
        {
            QImage tmpImage = rQImage.convertToFormat(QImage::Format_RGBA8888);
            uHashKey = UniqueImage(tmpImage);
            return(TransformQImage(uHashKey,tmpImage));
        }
        else
        {
            return(TransformQImage(uHashKey,rQImage));
        }
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

/// 根据
std::string CResourceLod::FindQImageKey(const QImage &rQImage)
{
    return(QString("%1_QImage").arg(UniqueImage(rQImage)).toLatin1().data());
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
        auto pNormals = new osg::Vec3Array(osg::Array::BIND_OVERALL);

        auto pDrawArray = new osg::DrawArrays(GL_TRIANGLE_STRIP,0,4);
        auto pGeometry = new osg::Geometry;
        pGeometry->setDataVariance(osg::Object::DYNAMIC);

        pVertexColor->at(0).set(1.f,1.f,1.f,1.f);
        pNormals->push_back(osg::Vec3f(0.f,0.f,1.f));
        pGeometry->setNormalArray(pNormals);

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
        m_mapImageNode[pImage] = pGeometry;
        return(pGeometry);
    }
}

/// 转换QImage成osgImage
osg::Image *CResourceLod::TransformQImage(uint uHashKey,const QImage &rQImage)
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
    m_mapQtImage[uHashKey] = image;
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
        pStateSete->getOrCreateUniform("openFlag",osg::Uniform::BOOL)->set(true);
    }
    else if(sFileName == "Atmosphere.glsl")
    {
        osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(pStateSete);
        if(nullptr != vp)
        {
            vp->setInheritShaders(false);
        }
        pStateSete->setRenderingHint(osg::StateSet::DEFAULT_BIN);
        pStateSete->setRenderBinDetails( -1, "RenderBin" );
        pStateSete->setAttributeAndModes( new osg::BlendFunc(GL_ONE,GL_ONE), osg::StateAttribute::ON );

        pStateSete->getOrCreateUniform("oe_sky_exposure", osg::Uniform::FLOAT)->set(1.f);
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
        pStateSete->setEventCallback(new ViewPortChanged(pStateSete->getUniform("cameraSize")));
    }
    else if(sFileName == "Pulse.glsl")
    {
        pStateSete->getOrCreateUniform("bIsOut",osg::Uniform::BOOL)->set(true);
        pStateSete->getOrCreateUniform("fPulseStartTime",osg::Uniform::FLOAT)->set((float)osg::Timer::instance()->time_s());
        pStateSete->getOrCreateUniform("fPulseIntervalTime",osg::Uniform::FLOAT)->set(1.f);
        pStateSete->getOrCreateUniform("fPulseStep",osg::Uniform::FLOAT)->set(.1f);
    }
}
