#include <QString>
#include <QDateTime>

#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Uniform>
#include <osg/Point>
#include <osg/BlendFunc>

#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "StarRender.h"
#include "GeodesicGrid.h"

static const float EYE_RESOLUTION (0.25f);
static const float MAX_LINEAR_RADIUS (8.f);

CStarRender::CStarRender(ISceneGraph *pSceneGraph, float fShowMaxMag)
    :m_pSceneGraph(pSceneGraph)
    ,m_fShowMaxMag(fShowMaxMag)
{
    m_StarTR.setWorldAdaptationLuminance(1.0);

    init();
}

/// 渲染回调
int CStarRender::renderFunc(int level, unsigned int index, const osg::Vec3 &c0,
                            const osg::Vec3 &c1, const osg::Vec3 &c2, void *context, int value)
{
    StarsRenderContext *sctx = (StarsRenderContext *) context;
    osg::Camera *camera = sctx->pOsgRenderInfo->getCurrentCamera();
    if (!camera)
        return Frustum::Out;

    if (value == Frustum::PartIn)
    {
        static osg::Vec3 unit_c0, unit_c1, unit_c2;
        unit_c0.set(c0);
        unit_c0.normalize();
        unit_c1.set(c1);
        unit_c1.normalize();
        unit_c2.set(c2);
        unit_c2.normalize();
        value = sctx->pBuilder->m_Frustum.checkFrustum(unit_c0, unit_c1,
                                                       unit_c2);
    }

    if (value > 0 && level >= 0)
    {
        for (size_t i = 0; i < sctx->pBuilder->m_vRenderVector.size(); i++)
        {
            if (sctx->pBuilder->m_vRenderVector[i]->getLevel() == level)
            {
                osg::Geometry *geom =
                        sctx->pBuilder->m_vRenderVector[i]->getGeometry(sctx->pBuilder->m_fShowMaxMag, index);
                if (geom)
                {
                    geom->draw(*sctx->pOsgRenderInfo);
                }

                break;
            }
        }
    }

    return value;
}


/// 设置网格
void CStarRender::setGeodesicGrid(ZoneArrayVector &vZoneData, GeodesicGrid *pGeodesicGrid
                                  , int nMaxLevel, const QMap<int, QString> &mapStarName, osg::Group *pRoot, double dJD)
{
    m_vRenderVector.clear();

    m_pGeodesicGrid = pGeodesicGrid;
    m_nMaxLevel = nMaxLevel;

    /// 清空所有的渲染结构体
    m_vRenderVector.clear();

    for(int i=0; i<vZoneData.size(); ++i)
    {
        osg::ref_ptr<CStarZoneArrayRenderer> render = new CStarZoneArrayRenderer(m_pSceneGraph,
                                                                                 vZoneData[i], dJD,m_pGeodesicGrid,mapStarName);


        const std::vector<osg::ref_ptr<osgText::Text> >& crStarNames = render->getStarNames();
        for(std::vector<osg::ref_ptr<osgText::Text> >::const_iterator itorName = crStarNames.begin();crStarNames.end() != itorName; ++itorName)
        {
            pRoot->addChild(*itorName);
        }
        m_vRenderVector.push_back(render);
    }
}

/// 重写绘制
void CStarRender::drawImplementation(osg::RenderInfo &renderInfo) const
{
    double fov, zNear, zFar, aspect;
    if (!renderInfo.getState()->getModelViewMatrix().getPerspective(fov,
                                                                    aspect, zNear, zFar))
    {
        fov = 45.0;
    }

    CStarRender* This = (CStarRender*)this;
    This->updateEye(fov);
    This->setupState();

    StarsRenderContext ctx;
    ctx.pBuilder = This;
    ctx.pOsgRenderInfo = &renderInfo;
    This->m_Frustum.update(renderInfo.getState()->getModelViewMatrix(),
                           renderInfo.getState()->getProjectionMatrix());

    /// 渲染星星
    m_pGeodesicGrid->visitTriangles(m_nMaxLevel, renderFunc, &ctx);
}

CStarRender::~CStarRender()
{
}

void CStarRender::init()
{
    osg::StateSet *state = getOrCreateStateSet();

    osg::ref_ptr<osg::Texture2D> starTexture1 = m_pSceneGraph->ResouceLoader()->LoadTexture("Space/pixmaps/asterism.png");
    state->setTextureAttributeAndModes(0, starTexture1.get());

    osg::ref_ptr<osg::Texture2D> starTexture2 = m_pSceneGraph->ResouceLoader()->LoadTexture("Space/pixmaps/star.png");
    state->setTextureAttributeAndModes(1, starTexture2.get());

    state->getOrCreateUniform("baseTexture", osg::Uniform::SAMPLER_2D)->set(0);
    state->getOrCreateUniform("tex", osg::Uniform::SAMPLER_2D)->set(1);

    /// 设置点大小
    state->setMode(GL_PROGRAM_POINT_SIZE,osg::StateAttribute::ON);
    state->setMode(GL_BLEND,osg::StateAttribute::ON);
}

void CStarRender::setupState()
{
    osg::StateSet *state = getOrCreateStateSet();

    state->getOrCreateUniform("lnfovFactor", osg::Uniform::FLOAT)->set(m_fLnfovFactor);
    state->getOrCreateUniform("lnInputScale", osg::Uniform::FLOAT)->set(m_StarTR.lnInputScale);
    state->getOrCreateUniform("lnOneOverMaxdL", osg::Uniform::FLOAT)->set(m_StarTR.lnOneOverMaxdL);
    state->getOrCreateUniform("fLinearScale", osg::Uniform::FLOAT)->set(m_fStarLinearScale);
    state->getOrCreateUniform("maxMag", osg::Uniform::FLOAT)->set(m_fShowMaxMag);
    state->getOrCreateUniform("alphaWaOverAlphaDa", osg::Uniform::FLOAT)->set(m_StarTR.alphaWaOverAlphaDa);
    state->getOrCreateUniform("lnTerm2", osg::Uniform::FLOAT)->set(m_StarTR.lnTerm2);
}

float CStarRender::pointSourceMagToLnLuminance(float mag) const
{
    return -0.92103 * (mag + 12.12331) + m_fLnfovFactor;
}

void CStarRender::computeRCMag(float mag, float rcMag[2], float scalar) const
{
    rcMag[0] = m_StarTR.adaptLuminanceScaledLn(pointSourceMagToLnLuminance(mag),scalar * 0.28);
    rcMag[0] *= m_fStarLinearScale * 0.185;

    if (rcMag[0] < 1)
    {
        rcMag[1] = rcMag[0] * rcMag[0];
        if (rcMag[0] < 0.5)
            rcMag[0] = 0.5f;
    }
    else
    {
        rcMag[1] = 1.0f;
        if (rcMag[0] > MAX_LINEAR_RADIUS)
        {
            rcMag[0] = MAX_LINEAR_RADIUS
                    + std::sqrt(1.f + rcMag[0] - MAX_LINEAR_RADIUS) - 1.f;
        }
    }
    rcMag[0] *= 3.5f;
}

float CStarRender::computeLimitMagnitude(float scalar) const
{
    float a = -26.f;
    float b = 30.f;
    float rcmag[2];
    float lim = 0.f;
    int safety = 0;
    while (std::fabs(lim - a) > 0.05)
    {
        computeRCMag(lim, rcmag, scalar);
        if (rcmag[0] <= 0.f)
        {
            float tmp = lim;
            lim = (a + lim) * 0.5;
            b = tmp;
        }
        else
        {
            float tmp = lim;
            lim = (b + lim) * 0.5;
            a = tmp;
        }
        ++safety;
        if (safety > 20)
        {
            lim = -99;
            break;
        }
    }
    return lim;
}
float CStarRender::computeLimitLuminance() const
{
    float a = 0.f;
    float b = 500000.f;
    float lim = 40.f;
    int safety = 0;
    float adaptL;
    while (std::fabs(lim - a) > 0.05)
    {
        adaptL = m_StarTR.adaptLuminanceScaled(lim);
        if (adaptL <= 0.05f) // Object considered not visible if its adapted scaled luminance<0.05
        {
            float tmp = lim;
            lim = (b + lim) * 0.5;
            a = tmp;
        }
        else
        {
            float tmp = lim;
            lim = (a + lim) * 0.5;
            b = tmp;
        }
        ++safety;
        if (safety > 30)
        {
            lim = 500000;
            break;
        }
    }
    return lim;
}

void CStarRender::updateEye(float fov)
{
    if (fov < 45)
        fov = 45;
    if (fov > 60)
        fov = 60;

    float powFactor = ::pow(60.f / fmax(0.7f,fov), 0.8f);
    m_StarTR.setInputScale(powFactor);

    m_fLnfovFactor = ::log(145800000.0f / (fov * fov)
                           / (EYE_RESOLUTION * EYE_RESOLUTION)/ powFactor
                           / 1.4f);

    m_fStarLinearScale = ::pow(42.0f,0.85f);

    m_fLimitMagnitude = computeLimitMagnitude(1.f);
    m_fLimitLuminance = computeLimitLuminance();
}
